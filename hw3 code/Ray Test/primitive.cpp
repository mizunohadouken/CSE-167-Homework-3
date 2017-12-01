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

	if (determinant < k_eps) return false; // ray is parallel or tri is backfacing
	if (fabs(determinant) < k_eps) return false; // ray is near 0, so parallel

	float det_inverse = 1 / determinant;
	glm::vec3 temp_vec_T = ray_shot.ray_origin - v0;
	float beta = (glm::dot(temp_vec_p, temp_vec_T)) * det_inverse;
	if (beta < 0 || beta > 1) return false; // not in triangle

	glm::vec3 temp_vec_Q = glm::cross(temp_vec_T, v0v1);
	float gamma = (glm::dot(temp_vec_Q, ray_shot.ray_dir)) * det_inverse;
	if (gamma < 0 || beta + gamma > 1) return false;

	out_t = glm::dot(temp_vec_Q, v0v2);
	return true;
}

/*
bool triangle::intersect(const ray ray_shot, float &out_t)
{

	// find denomiator of t
	float t_denominator = glm::dot(ray_shot.ray_dir, tri_normal);

	// if denominator of t is near zero, undefined so plane and ray are parallel
	if (fabs(t_denominator) < k_eps){return false;}
	
	float t_temp =  (glm::dot(v0, tri_normal) - glm::dot(ray_shot.ray_origin, tri_normal))/t_denominator;
		

	if (t_temp < 0){return false;}	// intesection is behind ray
	glm::vec3 Point_Hit = ray_shot.ray_origin + t_temp*ray_shot.ray_dir; // compute point hit on plane by ray

	*/

																		 /*
	glm::vec3 tmp_nb = glm::cross(v2 - Point_Hit, v0 - Point_Hit);
	glm::vec3 tmp_nc = glm::cross(v0 - Point_Hit, v1 - Point_Hit);

	float beta = glm::dot(tri_normal, tmp_nb) / glm::dot(tri_normal, tri_normal);
	float gamma = glm::dot(tri_normal, tmp_nc) / glm::dot(tri_normal, tri_normal);
	float alpha = 1.0 - beta - gamma;

	if (beta > -k_eps && beta < 1.0 + k_eps&& gamma > -k_eps && gamma < 1.0 + k_eps
		&& alpha > -k_eps && alpha < 1.0 + k_eps) {
		out_t = t_temp;
		return true;
	}
	else
		return false;
	*/


	/*
	// ray intersects plane defined by vertices
	// now check if intersection is inside triangle using barycentric coordinates
	glm::vec3 a = Point_Hit - v0,
			  b = v1 - v0,
			  c = v2 - v0;
	
	// compute barycentric coordinates
	float beta_gamma_denominator = b.x*c.y - b.y*c.x;
	if (beta_gamma_denominator == 0){return false;}
//	printf("made it"); // TODO remove

	float beta = (a.x*c.y - a.y*c.x) / beta_gamma_denominator,
	      gamma = (a.y*b.x - a.x*b.y) / beta_gamma_denominator,
		  alpha = 1 - beta - gamma;

	// check if alpha, beta, and gamma are between 0 and 1 inclusive
//	if ((alpha < k_eps) || (beta < k_eps) || (gamma < k_eps) || (alpha > 1) || (beta > 1) || (gamma > 1)) return false;
	if (beta >= k_eps && beta <= 1 
		&& gamma >= k_eps && gamma <= 1 
		&& alpha >= k_eps && alpha <= 1)
	{
		// if this point reached, hit point is within triangle and returns true
		out_t = t_temp;
		return true;
	}
		
	else false;

	}*/


