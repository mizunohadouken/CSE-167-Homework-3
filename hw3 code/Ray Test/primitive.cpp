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


bool triangle::intersect(const ray ray_shot, float &out_t)
{
	// find denomiator of t
	float t_denominator = glm::dot(ray_shot.ray_dir, tri_normal);

	if (fabs(t_denominator) < k_eps) return false; // if denominator of t is near zero, undefined so plane and ray are parallel
		
	float t_temp =  (glm::dot(v0, tri_normal) - glm::dot(ray_shot.ray_origin, tri_normal))/t_denominator;
	if (t_temp < 0) return false; // intesection is behind ray

	glm::vec3 Point_Hit = ray_shot.ray_origin + t_temp*ray_shot.ray_dir; // compute point hit on plane by ray

	// ray intersects plane defined by vertices
	// now check if intersection is inside triangle using barycentric coordinates
	glm::vec3 a = Point_Hit - v0,
			  b = v1 - v0,
			  c = v2 - v0;

	// compute barycentric coordinates
	float beta_gamma_denominator = b.x*c.y - b.y*c.x,
		  beta = (a.x*c.y - a.y*c.x) / beta_gamma_denominator,
		  gamma = (a.y*b.x - a.x*b.y) / beta_gamma_denominator,
		  alpha = 1 - beta - gamma;

	// check if alpha, beta, and gamma are between 0 and 1 inclusive
	if ((alpha < k_eps) || (beta < k_eps) || (gamma < k_eps)
	 || (alpha > 1) || (beta > 1) || (gamma > 1)) return false;

	// if this point reached, hit point is within triangle and returns true
	out_t = t_temp;
	return true;
}

