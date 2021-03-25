//
// Created by Lars on 18.03.2021.
//

#ifndef ENGINE_WORLD_H
#define ENGINE_WORLD_H

#include <entt/entt.hpp>
#include <vector>
#include <Components.h>

using namespace std;
using namespace entt;

class World {

public:

	World() {entities = make_unique<registry>(); }
	~World() = default;

	inline void addSystem(function<void(World&)> system){
		systems.push_back(system);
	}

	inline void removeSystem(function<void(World&)> system){}

	inline void process(){
		for (auto &system : systems)
			system(*this);
	}


	unique_ptr<registry> entities;
	vector<function<void(World&)>> systems;
};

namespace WorldUtils {

	#define DEFAULT_BACKGROUND_COLOR WHITE
	#define DEFAULT_LIGHT_COLOR WHITE

	static Mesh cubeMesh = Mesh();

	/**
	 * Creates a camera entity and returns it
	 * @param world The world used
	 * @param camera The camera struct used
	 * @param color The background color
	 * @return The camera entity
	 */
	inline entity createCamera(World &world, Camera &camera, Color color) {

		auto cameraEntity = world.entities->create();
		auto &cam = world.entities->emplace<Cam>(cameraEntity);
		auto &transform = world.entities->emplace<Transform>(cameraEntity);

		cam.camera = camera;
		cam.backgroundColor = color;
		transform.translation = camera.position;
		transform.scale = Vector3{1,1,1};
		transform.rotation = QuaternionIdentity();

		return cameraEntity;
	}

	/**
	 * Creates a default camera entity and returns it
	 * @param world The world used
	 * @return
	 */
	inline entity createDefaultCamera(World &world){

		// Setting up the camera
		Camera3D camera = { 0 };
		camera.position = (Vector3){ 80.0f, 60.0f, 80.0f };
		camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
		camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
		camera.fovy = 70.0f;
		camera.type = CAMERA_PERSPECTIVE;

		return createCamera(world, camera, DEFAULT_BACKGROUND_COLOR);
	}

	/**
	 * Creates a light entity and returns it
	 * @param world The world used
	 * @param light The light used
	 * @param shader The shader used for the light
	 * @return The light entity
	 */
	inline entity createLight(World &world, Light &light, Shader &shader){

		auto lightEntity = world.entities->create();
		auto &lux = world.entities->emplace<Lux>(lightEntity);
		auto &transform = world.entities->emplace<Transform>(lightEntity);
		auto &shadering = world.entities->emplace<Shadering>(lightEntity);

		lux.light = light;
		lux.type = static_cast<LightType>(light.type);
		shadering.shader = shader;

		transform.translation = light.position;
		transform.scale = Vector3{1,1,1};
		transform.rotation = QuaternionIdentity();

		return lightEntity;
	}

	/**
	 * Creates a default light and returns it
	 * @param world The world used
	 * @param shader The light shader used
	 * @return
	 */
	inline entity createDefaultLight(World &world, Shader &shader){

		auto light = CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 50, 50, 0 }, Vector3Zero(), DEFAULT_LIGHT_COLOR, shader);
		return createLight(world, light, shader);
	}

	inline entity createCube(World &world, const Vector3 &translation, const Quaternion &rotation, const Vector3 &scale, const Material &material){

		if(cubeMesh.triangleCount == 0) cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
		const auto entity = world.entities->create();

		auto &transform =  world.entities->emplace<Transform>(entity);
		transform.translation = translation;
		transform.rotation = rotation;
		transform.scale = scale;

		auto &renderer = world.entities->emplace<Renderer>(entity);
		renderer.mesh = cubeMesh;
		renderer.material = material;
		renderer.transform = MatrixIdentity();

		return entity;
	}
}

#endif //ENGINE_WORLD_H
