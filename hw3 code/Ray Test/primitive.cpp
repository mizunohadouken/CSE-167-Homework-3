#include "primitive.h"

primitive::primitive()
{
	prim_ambient = glm::vec3(.2f, .2f, .2f);  // TODO need this initialized? = glm::vec3(.2f, .2f, .2f);
}

primitive::~primitive()
{
}

bool primitive::intersect(const ray ray_shot, float & out_t)
{
	return false;
}

ray primitive::inv_transform_ray(const ray & ray_to_inv)
{
	glm::vec4 ray_org_hom = glm::vec4(ray_to_inv.ray_origin, 1.0f);
	glm::vec4 ray_dir_hom = glm::vec4(ray_to_inv.ray_dir, 0.0f);

	glm::vec4 inv_ray_org = m_transform_stack_inv * ray_org_hom;
	glm::vec4 inv_ray_dir = m_transform_stack_inv * ray_dir_hom;

	// demogonize origin and input origin and direction into new ray
	float inv_w = 1 / inv_ray_org.w;
	ray inv_ray(glm::vec3(inv_ray_org.x * inv_w, inv_ray_org.y * inv_w, inv_ray_org.z * inv_w),
				glm::vec3(inv_ray_dir.x, inv_ray_dir.y, inv_ray_dir.z));

	return inv_ray;
}

float primitive::revert_t(ray& trans_ray, float& inv_t, glm::vec3 ray_origin)
{
	glm::vec3 inv_ray_int = trans_ray.ray_origin + trans_ray.ray_dir*inv_t;
	glm::vec4 hom_inv_ray_int = glm::vec4(inv_ray_int, 1.0f);
	glm::vec4 hom_ray_int_transform = (m_transform_stack) * hom_inv_ray_int;

	float inv_ray_int_w = 1 / hom_ray_int_transform.w;
	glm::vec3 dehom_ray_int = glm::vec3(hom_ray_int_transform.x * inv_ray_int_w,
										hom_ray_int_transform.y * inv_ray_int_w,
										hom_ray_int_transform.z * inv_ray_int_w);

	float t = glm::length(dehom_ray_int - ray_origin);
	return t;
}


sphere::sphere(const glm::vec3 &center_con, const float &radius_con)
{
	center = center_con;
	radius = radius_con;
	radius_sq = radius*radius;
	prim_ambient = glm::vec3(.2f, .2f, .2f); // TODO default ambient
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

	prim_ambient = glm::vec3(.2f, .2f, .2f); // TODO default ambient
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