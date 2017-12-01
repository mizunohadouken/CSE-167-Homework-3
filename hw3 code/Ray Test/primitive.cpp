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
	else if (discrimnant == 0) out_x0 = out_x1 = -.5f *b / a;
	else
	{
		float temp_calc = (b > 0) ? // TODO check this formula of quadratic
			.5f * (-b + sqrt(discrimnant)) :
			.5f * (-b - sqrt(discrimnant));
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

	tri_normal = glm::cross((v1 - v0), (v2 - v0));
//	tri_normal = glm::normalize(glm::cross((v1 - v0), (v2 - v0))); // TODO check if unit norm needed
}



bool triangle::intersect(const ray ray_shot, float &out_t)
{
	glm::vec3 v0v1 = v1 - v0;
	glm::vec3 v0v2 = v2 - v0;
	glm::vec3 temp_vec_p = glm::cross(ray_shot.ray_dir, v0v2);
	float determinant = glm::dot(temp_vec_p, v0v1);

//	if (determinant < k_eps) return false; //  TODO comment out to disable culling, ray is parallel or tri is backfacing
	if (fabs(determinant) < k_eps) return false; // ray is near 0, so parallel

	float det_inverse = 1 / determinant;
	glm::vec3 temp_vec_T = ray_shot.ray_origin - v0;
	float beta = (glm::dot(temp_vec_p, temp_vec_T)) * det_inverse;
	if (beta < 0 || beta > 1) return false; // not in triangle

	glm::vec3 temp_vec_Q = glm::cross(temp_vec_T, v0v1);
	float gamma = (glm::dot(temp_vec_Q, ray_shot.ray_dir)) * det_inverse;
	if (gamma < 0 || beta + gamma > 1) return false;

	out_t = (glm::dot(temp_vec_Q, v0v2)) * det_inverse;
	return true;
}