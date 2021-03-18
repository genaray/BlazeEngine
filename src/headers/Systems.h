//
// Created by Lars on 18.03.2021.
//

#ifndef ENGINE_SYSTEMS_H
#define ENGINE_SYSTEMS_H

namespace Systems {

	struct BatchedDrawCall{

		Mesh &mesh;
		Material &material;

		bool operator==(const BatchedDrawCall &rhs) const {
			return mesh.triangleCount == rhs.mesh.triangleCount && material.maps == rhs.material.maps;
		}

		bool operator!=(const BatchedDrawCall &rhs) const {return !(rhs == *this);}
	};

	inline void shaderSystem(World &world){

		auto const cams = world.entities->group<const Transform, const Cam>();
		auto const shaders = world.entities->view<const Shadering>();

		for (const auto &camEntity : cams) {

			const auto camTransform = world.entities->get<Transform>(camEntity);
			const auto cam = world.entities->get<Cam>(camEntity);

			for (const auto &entity : shaders) {

				const auto shadering = world.entities->get<Shadering>(entity);
				const auto shader = shadering.shader;

				// Update the shaders with the camera view position
				const float cameraPos[3] = {camTransform.translation.x, camTransform.translation.y, camTransform.translation.z};
				SetShaderValue(shader, shader.locs[LOC_VECTOR_VIEW], cameraPos, UNIFORM_VEC3);
			}
		}
	}

	inline void renderSystem(World &world) {

		auto const cams = world.entities->view<const Cam>();
		auto const renders = world.entities->view<const Transform, const Renderer>();

		/*
		auto index = 0;
		for(const auto &entity : renders) {

			const auto &transform = world.entities->get<Transform>(entity);
			auto &renderer = world.entities->get<Renderer>(entity);

			const auto translation = MatrixTranslate(transform.translation.x, transform.translation.y, transform.translation.z);
			renderer.transform = MatrixMultiply(MatrixIdentity(), translation);

			const auto kvp = make_pair(renderer.material, renderer.mesh);
			batched.emplace(kvp, vector<Matrix>());
			index++;
		}*/


		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		for (const auto &entity : cams) {

			const auto &cam = world.entities->get<Cam>(entity);
			ClearBackground(cam.backgroundColor);

			BeginMode3D(cam.camera);

			EndMode3D();
		}
		EndDrawing();
	}
}

#endif //ENGINE_SYSTEMS_H
