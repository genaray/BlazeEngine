//
// Created by Lars on 18.03.2021.
//

#ifndef ENGINE_SYSTEMS_H
#define ENGINE_SYSTEMS_H

#include <vector>
#include <queue>

namespace SystemUtils {

	/** A struct which stores references to the mesh and material for batching them **/
	struct BatchedDrawCall {

		const Mesh &mesh;
		const Material &material;

		bool operator==(const BatchedDrawCall &rhs) const {
			return mesh.triangleCount == rhs.mesh.triangleCount && material.params == rhs.material.params;
		}
		bool operator!=(const BatchedDrawCall &rhs) const { return !(rhs == *this); }
	};
}


namespace std {

	using namespace SystemUtils;

	template<>
	struct hash<BatchedDrawCall> {
		size_t operator()(BatchedDrawCall const& t) const {
			return (size_t)t.mesh.vaoId+t.material.shader.id;
		}
	};
}


namespace Systems {

	using namespace SystemUtils;

	// Two maps for batching the draw calls with same meshes and materials
	static auto batchProcessor = unordered_map<BatchedDrawCall, size_t>();
	static auto batches = unordered_map<BatchedDrawCall, vector<Matrix>*>();

	/**
	 * The shader system taking care of passing world attributes into the shaders.
	 * @param world
	 */
	inline void shaderSystem(World &world){

		// Get cam and shaders
		auto const cams = world.entities->group<const Transform, const Cam>();
		auto const shaders = world.entities->view<const Shadering>();

		// Apply cam attributes to the shaders
		for (const auto &camEntity : cams) {

			const auto camTransform = world.entities->get<Transform>(camEntity);
			for (const auto &entity : shaders) {

				const auto shadering = world.entities->get<Shadering>(entity);
				const auto shader = shadering.shader;

				// Update the shaders with the camera view position
				const float cameraPos[3] = {camTransform.translation.x, camTransform.translation.y, camTransform.translation.z};
				SetShaderValue(shader, shader.locs[LOC_VECTOR_VIEW], cameraPos, UNIFORM_VEC3);
			}
		}
	}

	/**
	 * The render system, takes care of batching meshes, materials and contains the render loop itself.
	 * @param world
	 */
	inline void renderSystem(World &world) {

		// Get cams and instancedRenderers
		auto const cams = world.entities->view<const Cam>();
		auto const renderers = world.entities->view<const Transform, const Renderer>();
		auto const nonInstanced = world.entities->view<const Transform, const Renderer>(exclude<InstancedRenderer>);
		auto const instancedRenderers = world.entities->view<const Transform, const Renderer, const InstancedRenderer>();

		// Resetting batches
		for (auto const& [key, val] : batchProcessor)
			batchProcessor[key] = 0;

		// Counting possible batches
		for(const auto &entity : instancedRenderers) {

			// Receive components
			const auto &renderer = world.entities->get<Renderer>(entity);

			// When theres a key with that batched draw call, insert it into the list... otherwhise create a new one
			const auto batch = BatchedDrawCall{renderer.mesh, renderer.material};
			batchProcessor[batch]++;
		}

		// Reserving space for possible batches
		for (auto const& [key, val] : batchProcessor) {

			// When there are also batches for that key, resize the array to the new size.
			// Otherwhise, insert a new allocated vector into it.
			const auto found = batches.find(key);
			if(found != batches.end()){

				auto matrices = found->second;

				// Either reserve new matrices or remove the obsolete ones
				if (matrices->size() > val)
					matrices->reserve(val);
				else if (matrices->size() < val){
					matrices->resize(val);
					matrices->shrink_to_fit();
				}

				matrices->clear();
			} else {
				static auto matrices = new vector<Matrix>();
				matrices->reserve(val);
				batches[key] = matrices;
			}
		}

		// Calculating the matrix being passed to opengl and pushes that one into the batches.
		for(const auto &entity : renderers) {

			// Receive components
			const auto &transform = world.entities->get<Transform>(entity);
			auto &renderer = world.entities->get<Renderer>(entity);
			const auto isIstanced = world.entities->has<InstancedRenderer>(entity);

			// Apply transform for calculating the matrix
			const auto translation = MatrixTranslate(transform.translation.x, transform.translation.y, transform.translation.z);
			const auto matrix = MatrixMultiply(MatrixIdentity(), translation);

			// When theres a key with that batched draw call, insert it into the list... otherwhise create a new one
			if (isIstanced) {
				const auto batch = BatchedDrawCall{renderer.mesh, renderer.material};
				batches[batch]->push_back(matrix);
			} else renderer.transform = matrix;
		}

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		for (const auto &camEntity : cams) {

			const auto &cam = world.entities->get<Cam>(camEntity);
			ClearBackground(cam.backgroundColor);
			BeginMode3D(cam.camera);

				// Batched rendering loop
				for (auto const& [key, val] : batches)
					rlDrawMeshInstanced(key.mesh, key.material, (Matrix *) (val->data()), val->size());

				// Normal rendering loop
				for(const auto &entity : nonInstanced) {

					const auto &renderer = world.entities->get<Renderer>(entity);
					rlDrawMesh(renderer.mesh, renderer.material, renderer.transform);
				}
			EndMode3D();
		}
		EndDrawing();
	}
}


#endif //ENGINE_SYSTEMS_H
