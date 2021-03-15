//
// Created by Lars on 14.03.2021.
//

#include "VectorUtils.h"

Vector3 VectorUtils::randomVector3(float lowest, float highest) {

	auto randomX = static_cast<float>(GetRandomValue(lowest, highest));
	auto randomY = static_cast<float>(GetRandomValue(lowest, highest));
	auto randomZ = static_cast<float>(GetRandomValue(lowest, highest));
	return Vector3{randomX, randomY, randomZ};
}

Vector3 VectorUtils::randomVector3(Vector3 lowest, Vector3 highest) {

	auto randomX = static_cast<float>(GetRandomValue(0, 20));
	auto randomY = static_cast<float>(GetRandomValue(0, 20));
	auto randomZ = static_cast<float>(GetRandomValue(0, 20));
	return Vector3{randomX, randomY, randomZ};
}
