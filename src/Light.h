#ifndef LIGHT_H_
#define LIGHT_H_

#include <stack>
#include <memory>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {
public:
	//position
	glm::vec3 pos;
	//color
	glm::vec3 color;
	Light();
};


#endif