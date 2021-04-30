#pragma once
#ifndef HIT_H_
#define HIT_H_

#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Object.h"

class Hit
{
public:
	Hit() : x(0), n(0), t(0) {}
	Hit(const glm::vec3& x, const glm::vec3& n, float t) { this->x = x; this->n = n; this->t = t; }
	glm::vec3 x; // position
	glm::vec3 n; // normal
	float t; // distance
	glm::vec3 kD;
	glm::vec3 kS;
	glm::vec3 kA;
	float shine;
	bool hasHit = false;
};




#endif