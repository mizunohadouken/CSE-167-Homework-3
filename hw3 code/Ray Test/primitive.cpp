#include "primitive.h"

primitive::primitive()
{
}

primitive::~primitive()
{
}

bool primitive::intersect(const ray ray_shot, float & out_t)
{
	return false;
}

sphere::sphere(const glm::vec3 &center_con, const float &radius_con)
{
	center = center_con;
	radius = radius_con;
	radius_sq = radius*radius;
}

bool sphere::intersect(const ray ray_shot, float & out_t)
{
	float a = glm::dot(ray_shot.ray_dir, ray_shot.ray_dir),
		b = 2 * (glm::dot(ray_shot.ray_dir, (ray_shot.ray_origin - center))),
		c = (glm::dot((ray_shot.ray_origin - center), (ray_shot.ray_origin - center))) - radius_sq,
		out_t0,
		out_t1;

	if (!solve_quadratic(a, b, c, out_t0, out_t1)) return false;

	if (out_t0 > out_t1) std::swap(out_t0, out_t1);
	if (out_t0 < 0)
	{
		out_t0 = out_t1;
		if (out_t0 < 0) return false; // both quadratic solutions are neg, so intersection is behind camera
	}
	out_t = out_t0;
	return true;
}

bool sphere::solve_quadratic(const float & a, const float & b, const float & c, float & out_x0, float & out_x1)
{
	float discrimnant = b*b - 4 * a*c;

	if (discrimnant < 0) return false;
	else if (discrimnant = 0) out_x0 = out_x1 = -.5 *b / a;
	else
	{
		float temp_calc = (b > 0) ? // TODO check this formula of quadratic
			.5 * (-b + sqrt(discrimnant)) :
			.5 * (-b - sqrt(discrimnant));
		out_x0 = temp_calc / a;
		out_x1 = c / temp_calc;
	}
	if (out_x0 > out_x1) { std::swap(out_x0, out_x1); }
	return true;
}

float sphere::get_radius()
{
	return radius;
}


triangle::triangle(glm::vec3 &v0_con, glm::vec3 &v1_con, glm::vec3 &v2_con)
{
	v0 = v0_con;
	v1 = v1_con;
	v2 = v2_con;

	tri_normal = glm::normalize(glm::cross((v1 - v0), (v2 - v0))); // unit norm
}

/*
bool triangle::intersect(const ray ray_shot, float &out_t)
{
	// check if ray intersects plane (find t)


	// check if t<0, if so, return false (intersection not in view)

	// t >=0, find if ray is inside triangle by solving barycentric equation
}
*/
