#pragma once
#include "glm.hpp"

class lights
{
public:
	lights();
	~lights();

	glm::vec3 light_direction;


private:

};

class point_light :public lights
{

};

class directional_light :public lights
{

};
