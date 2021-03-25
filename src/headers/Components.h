//
// Created by Lars on 18.03.2021.
//

#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

/** A struct which represents a camera**/
struct Cam{
	Camera camera;
	Color backgroundColor;
};

/** A struct which represents a light source**/
struct Lux{
	Light light;
	LightType type;
};

/** A struct which represents an object with an shader attached **/
struct Shadering{
	Shader shader;
};

/** A renderer attached to an object for rendering it**/
struct Renderer{
	Mesh mesh;
	Material material;
	Matrix transform;
	Model test;
};

/** Marks an entity for instanced rendering **/
struct InstancedRenderer{};

#endif //ENGINE_COMPONENTS_H
