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

	// Ambient light level
	auto ambientLoc = GetShaderLocation(shader, "ambient");
	auto shaderValue = new float[4]{ 0.2f, 0.2f, 0.2f, 1.0f };
	SetShaderValue(shader, ambientLoc, shaderValue, UNIFORM_VEC4);

	// Setting up the camera
	WorldUtils::createDefaultCamera(world);
	WorldUtils::createDefaultLight(world, shader);
	world.addSystem(Systems::shaderSystem);
	world.addSystem(Systems::renderSystem);

	auto material = LoadMaterialDefault();
	material.shader = shader;
	material.maps[MAP_DIFFUSE].color = RED;

	// Creating the cube entities
	auto cubeVectors = EnvironmentUtils::cube(50);
	for (auto &vec : *cubeVectors) {

		const auto entity = WorldUtils::createCube(world, vec, {0,0,0,0}, {1,1,1}, material);
		world.entities->emplace<InstancedRenderer>(entity);
	}

	auto otherMaterial = LoadMaterialDefault();
	otherMaterial.shader = shader;
	otherMaterial.maps[MAP_DIFFUSE].color = RED;

	const auto entity = WorldUtils::createCube(world, {-10,-10,-10}, {0,0,0,0}, {1,1,1}, otherMaterial);
	auto test = world.entities->get<Renderer>(entity);
	test.mesh = GenMeshCube(1.0f, 1.0f, 1.0f);

	// Main game loop
	while (!WindowShouldClose())
		world.process();

	CloseWindow();        // Close window and OpenGL context
	return 0;
}
