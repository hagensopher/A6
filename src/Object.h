#pragma once
#ifndef OBJECT_H_
#define OBJECT_H_


#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include "Hit.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object
{
public:
	Object();
	~Object();
	glm::vec3 pos; //x y z
	glm::vec3 kD;
	glm::vec3 kS;
	glm::vec3 kA;
	float shine;
	glm::vec3 scale;
private:

};

class Plane : public Object {
public:
	void PlaneIntersection();
};
class Sphere : public Object {
public:
	//lab 13 task 3
	void SphereIntersection(Object object, std::vector<Hit>& hits, glm::vec3 rayDir, glm::vec3 rayOri) {
		//hits.clear();
		float a, b, c, d = 0;
		glm::vec3 Pw = rayOri;
		glm::vec3 Vw = glm::normalize(rayDir);
		//std::cout << "the Vw is " << rayDir.x << ',' << rayDir.y << ',' << rayDir.z << std::endl;
		//std::cout << "the Pw is " << rayOri.x << ',' << rayOri.y << ',' << rayOri.z << std::endl;
		glm::vec3 Xpoint;

		a = glm::dot(Vw, Vw);
		//std::cout << "a is " << a << std::endl;
		b =  glm::dot((Pw - object.pos), 2.0f * Vw);
		//std::cout << "b is " << b << std::endl;
		c = glm::dot(Pw-object.pos, Pw-object.pos) - 1.0f;
		//std::cout << "c is " << c << std::endl;
		d = (b*b) - (4.0 * a * c);
		//std::cout << "d is " << d << std::endl;
		if (d > 0) {
			
			float t2 = (-b + sqrt(d)) / (2.0f * a); //point 1
			
			Xpoint = Pw + (t2 * Vw);
			Hit sphereHit1;
			sphereHit1.x = Xpoint;
			sphereHit1.n = (Xpoint-object.pos)/object.scale.x;
			sphereHit1.t = t2;
			sphereHit1.kA = object.kA;
			sphereHit1.kD = object.kD;
			sphereHit1.kS = object.kS;
			sphereHit1.shine = object.shine;
			//hits.push_back(sphereHit1);
			Xpoint = glm::vec3(0.0f, 0.0f, 0.0f);
			float t3 = (-b - sqrt(d)) / (2.0f * a); //point 2
			
			Xpoint = Pw + (t3 * Vw);
			Hit sphereHit2;
			sphereHit2.x = Xpoint;
			sphereHit2.n = (Xpoint - object.pos) / object.scale.x;
			sphereHit2.t = t3;
			sphereHit2.kA = object.kA;
			sphereHit2.kD = object.kD;
			sphereHit2.kS = object.kS;
			sphereHit2.shine = object.shine;
			//hits.push_back(sphereHit2);
			if (t2 < 0 || t3 < 0) {
				return;
			}
			if (t2 < t3) {
				hits.push_back(sphereHit1);
			}
			else {
				hits.push_back(sphereHit2);
			}
		}
	}
};
class Ellipse : public Object {
	//lab 13 task 4
	void EllipseIntersection();
};

#endif // !OBJECT_H_
