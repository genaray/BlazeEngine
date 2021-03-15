//
// Created by Lars on 14.03.2021.
//

#include "headers/ColorUtils.h"

Color ColorUtils::randomColor() {

	auto randomX = static_cast<unsigned char>(GetRandomValue(0, 255));
	auto randomY = static_cast<unsigned char>(GetRandomValue(0, 255));
	auto randomZ = static_cast<unsigned char>(GetRandomValue(0, 255));
	auto randomA = static_cast<unsigned char>(GetRandomValue(0, 255));

	return Color{randomX, randomY, randomZ, randomA};
}
