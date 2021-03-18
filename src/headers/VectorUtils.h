//
// Created by Lars on 14.03.2021.
//

#ifndef ENGINE_VECTORUTILS_H
#define ENGINE_VECTORUTILS_H


#include <raylib.h>

class VectorUtils {
public:
	static inline Vector3 randomVector3(float lowest, float highest){

		auto randomX = static_cast<float>(GetRandomValue(lowest, highest));
		auto randomY = static_cast<float>(GetRandomValue(lowest, highest));
		auto randomZ = static_cast<float>(GetRandomValue(lowest, highest));

		return Vector3{randomX, randomY, randomZ};
	}

	static inline Vector3 randomVector3(Vector3 lowest, Vector3 highest){

		auto randomX = static_cast<float>(GetRandomValue(0, 20));
		auto randomY = static_cast<float>(GetRandomValue(0, 20));
		auto randomZ = static_cast<float>(GetRandomValue(0, 20));
		return Vector3{randomX, randomY, randomZ};
	}
};


#endif //ENGINE_VECTORUTILS_H
