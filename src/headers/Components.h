//
// Created by Lars on 18.03.2021.
//

#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

enum Primitives{
	CUBE,
	SPHERE
};

struct Cam{
	Camera camera;
	Color backgroundColor;
};

struct Lux{
	Light light;
	LightType type;
};

struct Shadering{
	Shader shader;
};

struct PrimitiveRenderer{
	Primitives type;
	Color color;
};

struct Renderer{
	Mesh mesh;
	Material material;
};

#endif //ENGINE_COMPONENTS_H
