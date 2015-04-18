#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP

#include <glm/glm.hpp>

namespace fumasu
{

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
};

};

#endif