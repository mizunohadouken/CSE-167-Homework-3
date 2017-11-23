#pragma once
#include <math.h>
#include "glm.hpp"
#include "camera.h"
#include <algorithm>


class primitive
{
public:
	primitive();
	virtual ~primitive();
	virtual bool intersect(const ray ray_shot, float &out_t);

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
