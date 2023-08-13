#pragma once
#include "TransformObject.h"

//  Base class for any object that needs a transform.
//
TransformObject::TransformObject() {
	pos = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rot = 0;
}

void TransformObject::setPosition(const glm::vec3 p) {
	pos = p;
}
