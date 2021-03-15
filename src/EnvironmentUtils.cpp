//
// Created by Lars on 14.03.2021.
//

#include <EnvironmentUtils.h>
#include <cmath>

unique_ptr<vector<Vector3>> EnvironmentUtils::cube(short size) {

	auto vectors = make_unique<vector<Vector3>>();
	vectors->reserve(pow(size, 3));

	for (auto xindex = 0; xindex < size; xindex++) {
		for (auto yindex = 0; yindex < size; yindex++) {
			for (auto zindex = 0; zindex < size; zindex++) {

				auto x = static_cast<float>(xindex);
				auto y = static_cast<float>(yindex);
				auto z = static_cast<float>(zindex);
				auto vector = Vector3{x, y, z};
				vectors->push_back(vector);
			}
		}
	}

	return vectors;
}

