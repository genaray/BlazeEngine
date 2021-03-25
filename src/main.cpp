#include <iostream>
#include <raylib/raylib.h>
#include <entt/entt.hpp>
#include <raymath.h>
#include <ColorUtils.h>
#include <EnvironmentUtils.h>
#include <rlgl.h>
#include <raylib.h>

#define RLIGHTS_IMPLEMENTATION
#include <raylib/rlights.h>
#include <World.h>
#include <Components.h>
#include <Systems.h>

using namespace std;
using namespace entt;

static const int width = 800;
static const int height = 800;

static World world;

int main() {

	// Creating window
	InitWindow(width, height, "Engine");
	SetTargetFPS(60);

	auto shader = LoadShader("../resources/shaders/base_lighting_instanced.vs","../resources/shaders/lighting.fs");

	// Get some shader loactions
	shader.locs[LOC_MATRIX_MVP] = GetShaderLocation(shader, "mvp");
	shader.locs[LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
	shader.locs[LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instance");

	auto material = LoadMaterialDefault();
	material.shader = shader;
	material.maps[MAP_DIFFUSE].color = RED;

	// Ambient light level
	auto ambientLoc = GetShaderLocation(shader, "ambient");
	auto shaderValue = new float[4]{ 0.2f, 0.2f, 0.2f, 1.0f };
	SetShaderValue(shader, ambientLoc, shaderValue, UNIFORM_VEC4);

	// Setting up the camera
	WorldUtils::createDefaultCamera(world);
	WorldUtils::createDefaultLight(world, shader);
	world.addSystem(Systems::shaderSystem);
	world.addSystem(Systems::renderSystem);

	// Creating the cube entities
	auto cubeVectors = EnvironmentUtils::cube(50);
	auto mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
	for (auto &vec : *cubeVectors) {

		const auto entity = world.entities->create();

		auto &transform =  world.entities->emplace<Transform>(entity);
		transform.translation = vec;
		transform.rotation = QuaternionIdentity();
		transform.scale = Vector3{1,1,1};

		auto &renderer = world.entities->emplace<Renderer>(entity);
		renderer.mesh = mesh;
		renderer.material = material;
	}


	// Main game loop
	while (!WindowShouldClose()){

		/*const auto entitySize = entities.size();
		auto index = 0;
		for(const auto &entity : entities) {

			const auto &transform = registry.get<Transform>(entity);
			auto &renderer = registry.get<RenderMatrix>(entity);

			const auto translation = MatrixTranslate(transform.translation.x, transform.translation.y, transform.translation.z);
			renderer.transform = MatrixMultiply(MatrixIdentity(), translation);
			index++;
		}

		const auto &transforms = entities.raw<const RenderMatrix>();*/

		// Draw
		//----------------------------------------------------------------------------------

		world.process();
		/*BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

			rlDrawMeshInstanced(mesh, material, (Matrix*)(transforms), entitySize);

		EndMode3D();
		EndDrawing();*/
	}

	CloseWindow();        // Close window and OpenGL context
	return 0;
}
