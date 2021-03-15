//
// Created by Lars on 14.03.2021.
//

#ifndef ENGINE_VECTORUTILS_H
#define ENGINE_VECTORUTILS_H


#include <raylib.h>

class VectorUtils {
public:
	static Vector3 randomVector3(float lowest, float highest);
	static Vector3 randomVector3(Vector3 lowest, Vector3 highest);
};


#endif //ENGINE_VECTORUTILS_H
