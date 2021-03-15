#include <iostream>
#include <raylib/raylib.h>
#include <entt/entt.hpp>
#include <raymath.h>
#include <ColorUtils.h>
#include <VectorUtils.h>
#include <EnvironmentUtils.h>
#include <ModelUtils.h>
#include <rlgl.h>

using namespace std;
using namespace entt;

const int width = 800;
const int height = 800;

enum Primitives{
	CUBE,
	SPHERE
};

struct PrimitiveRenderer{
	Primitives type;
	Color color;
	Model model;
};

struct Velocity{
	Vector3 velocity;
};

int main() {

	// Creating window
	InitWindow(width, height, "Engine");
	SetTargetFPS(60);

	// Setting up the camera
	Camera3D camera = { 0 };
	camera.position = (Vector3){ 30.0f, 20.0f, 30.0f };
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 70.0f;
	camera.type = CAMERA_PERSPECTIVE;

	// Creating the cube entities
	entt::registry registry;
	auto entities = registry.view<const Transform, const PrimitiveRenderer>();
	auto cubeVectors = EnvironmentUtils::cube(20);

	auto mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
	for (auto &vec : *cubeVectors) {

		const auto entity = registry.create();

		auto &transform = registry.emplace<Transform>(entity);
		transform.translation = vec;
		transform.rotation = QuaternionIdentity();
		transform.scale = Vector3{1,1,1};

		auto &renderer = registry.emplace<PrimitiveRenderer>(entity);
		renderer.type = Primitives::CUBE;
		renderer.color = ColorUtils::randomColor();

		renderer.model = LoadModelFromMesh(mesh);
	}

	// Main game loop
	while (!WindowShouldClose()){

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode3D(camera);
		for(const auto &entity : entities) {

			const auto &transform = registry.get<Transform>(entity);
			const auto &renderer = registry.get<PrimitiveRenderer>(entity);

			DrawModel(renderer.model, transform.translation, 1.0f, renderer.color);
		}
		EndMode3D();
		EndDrawing();
	}

	CloseWindow();        // Close window and OpenGL context
	return 0;
}
