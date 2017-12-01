#pragma once
#include <math.h>
#include "glm.hpp"
#include "camera.h"
#include <algorithm>

const float k_eps = 1e-8; // TODO check this value, might change to 0
//const float k_eps = 0;


class primitive
{
public:
	primitive();
	virtual ~primitive();
	virtual bool intersect(const ray ray_shot, float &out_t);

	// material properties
	glm::vec3 prim_diffuse, prim_specular, prim_emission, prim_ambient;  // TODO need this initialized? = glm::vec3(.2f, .2f, .2f);
	float prim_shininess;
	

private:
};

class sphere : public primitive
{
public:
	sphere(const glm::vec3 &center_con, const float &radius_con);
	bool intersect(const ray ray_shot, float &out_t);
	static bool solve_quadratic(const float &a, const float &b, const float &c, float &out_x0, float &out_x1);
	float get_radius();

private:
	glm::vec3 center;
	float radius;
	float radius_sq;
};

class triangle : public primitive
{
public:
	triangle(glm::vec3 &v0_con, glm::vec3 &v1_con, glm::vec3 &v2_con);
	bool intersect(const ray ray_shot, float &out_t);

	glm::vec3 v0, v1, v2, tri_normal;
private:
};