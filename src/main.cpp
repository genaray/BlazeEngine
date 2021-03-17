#include <iostream>
#include <raylib/raylib.h>
#include <entt/entt.hpp>
#include <raymath.h>
#include <ColorUtils.h>
#include <VectorUtils.h>
#include <EnvironmentUtils.h>
#include <ModelUtils.h>
#include <rlgl.h>
#include <raylib.h>

#define RLIGHTS_IMPLEMENTATION
#include "raylib/rlights.h"

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
};

struct Renderer{
	Mesh mesh;
	Material material;
};

struct RenderMatrix{
	Matrix transform;
};

int main() {

	// Creating window
	InitWindow(width, height, "Engine");
	SetTargetFPS(60);

	// Setting up the camera
	Camera3D camera = { 0 };
	camera.position = (Vector3){ 80.0f, 60.0f, 80.0f };
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 70.0f;
	camera.type = CAMERA_PERSPECTIVE;

	auto shader = LoadShader("../resources/shaders/base_lighting_instanced.vs","../resources/shaders/lighting.fs");

	// Get some shader loactions
	shader.locs[LOC_MATRIX_MVP] = GetShaderLocation(shader, "mvp");
	shader.locs[LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
	shader.locs[LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instance");

	auto material = LoadMaterialDefault();
	material.shader = shader;
	material.maps[MAP_DIFFUSE].color = RED;

	// Creating the cube entities
	registry registry;
	auto entities = registry.group<const Transform, const RenderMatrix>();
	auto cubeVectors = EnvironmentUtils::cube(50);

	auto mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
	for (auto &vec : *cubeVectors) {

		const auto entity = registry.create();

		auto &transform = registry.emplace<Transform>(entity);
		transform.translation = vec;
		transform.rotation = QuaternionIdentity();
		transform.scale = Vector3{1,1,1};

		auto &renderer = registry.emplace<Renderer>(entity);
		renderer.mesh = mesh;
		renderer.material = material;

		auto &rendererMatrix = registry.emplace<RenderMatrix>(entity);
		rendererMatrix.transform = MatrixIdentity();
	}

	// Ambient light level
	auto ambientLoc = GetShaderLocation(shader, "ambient");
	auto shaderValue = new float[4]{ 0.2f, 0.2f, 0.2f, 1.0f };
	SetShaderValue(shader, ambientLoc, shaderValue, UNIFORM_VEC4);

	CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 50, 50, 0 }, Vector3Zero(), WHITE, shader);

	// Main game loop
	while (!WindowShouldClose()){

		// Update the light shader with the camera view position
		const float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
		SetShaderValue(shader, shader.locs[LOC_VECTOR_VIEW], cameraPos, UNIFORM_VEC3);

		const auto entitySize = entities.size();
		auto index = 0;
		for(const auto &entity : entities) {

			const auto &transform = registry.get<Transform>(entity);
			auto &renderer = registry.get<RenderMatrix>(entity);

			const auto translation = MatrixTranslate(transform.translation.x, transform.translation.y, transform.translation.z);
			renderer.transform = MatrixMultiply(MatrixIdentity(), translation);
			index++;
		}

		const auto transforms = entities.raw<const RenderMatrix>();

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode3D(camera);
			rlDrawMeshInstanced(mesh, material, (Matrix*)(transforms), entitySize);
		EndMode3D();
		EndDrawing();
	}

	CloseWindow();        // Close window and OpenGL context
	return 0;
}
