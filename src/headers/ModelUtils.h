//
// Created by Lars on 15.03.2021.
//

#ifndef ENGINE_MODELUTILS_H
#define ENGINE_MODELUTILS_H


#include <raylib.h>
#include <memory>
#include <utility>

using namespace std;

class ModelUtils {
public:
	static Model merge(const Model &first, const Model &second);
};


#endif //ENGINE_MODELUTILS_H
