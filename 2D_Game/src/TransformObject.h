#pragma once
#include "ofMain.h"

//  Kevin M. Smith - CS 134 SJSU
//

//  Base class for any object that needs a transform.
//
class TransformObject {
public:
	TransformObject();
	glm::vec3 pos, scale;
	float	rot;
	bool	bSelected;
	void setPosition(const glm::vec3);
};