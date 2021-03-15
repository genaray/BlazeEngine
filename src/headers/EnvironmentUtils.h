//
// Created by Lars on 14.03.2021.
//

#ifndef ENGINE_ENVIRONMENTUTILS_H
#define ENGINE_ENVIRONMENTUTILS_H


#include <raylib.h>
#include <vector>
#include <memory>

using namespace std;

class EnvironmentUtils {
public:

	static unique_ptr<vector<Vector3>> cube(short size);
};

#endif //ENGINE_ENVIRONMENTUTILS_H
