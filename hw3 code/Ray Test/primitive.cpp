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
	std::cerr << "Unknown primitive calling intersect method\n";
	return false;
}

glm::vec3 primitive::get_normal(glm::vec3& intersect_point) const
{
	std::cerr << "Unknown primitive calling get_normal method\n";
	return glm::vec3();
}

ray primitive::inv_transform_ray(const ray & ray_to_inv) const
{
	glm::vec4 ray_org_hom = glm::vec4(ray_to_inv.ray_origin, 1.0f);
	glm::vec4 ray_dir_hom = glm::vec4(ray_to_inv.ray_dir, 0.0f);

	glm::vec4 ray_org_obj_space = m_transform_stack_inv * ray_org_hom;
	glm::vec4 ray_dir_obj_space = m_transform_stack_inv * ray_dir_hom;

	// demogonize origin and input origin and direction into new ray
	float inv_w = 1 / ray_org_obj_space.w;
	ray inv_ray(glm::vec3(ray_org_obj_space.x * inv_w, ray_org_obj_space.y * inv_w, ray_org_obj_space.z * inv_w),
				glm::vec3(ray_dir_obj_space.x, ray_dir_obj_space.y, ray_dir_obj_space.z));

	return inv_ray;
}

float primitive::revert_t(ray& trans_ray, float& inv_t, glm::vec3 ray_origin, glm::vec3& prim_int_world_space)
{
	glm::vec3 inv_ray_int = trans_ray.ray_origin + trans_ray.ray_dir*inv_t;
	glm::vec4 hom_inv_ray_int = glm::vec4(inv_ray_int, 1.0f);
	glm::vec4 hom_ray_int_transform = (m_transform_stack) * hom_inv_ray_int;

	float inv_ray_int_w = 1 / hom_ray_int_transform.w;
	prim_int_world_space = glm::vec3(hom_ray_int_transform.x * inv_ray_int_w,
						   hom_ray_int_transform.y * inv_ray_int_w,
						   hom_ray_int_transform.z * inv_ray_int_w);

	float t = glm::length(prim_int_world_space - ray_origin);
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
	/*
	float a = glm::dot(ray_shot.ray_dir, ray_shot.ray_dir),
		b = 2 * (glm::dot(ray_shot.ray_dir, (ray_shot.ray_origin - center))),
		c = (glm::dot((ray_shot.ray_origin - center), (ray_shot.ray_origin - center))) - radius_sq,
		out_t0,
		out_t1;

	float discriminant = (b * b) - (4 * a *c);
	float sqrt_dis = sqrt(discriminant);

	if (discriminant < 0) // no real roots
	{
		return false;
	}

	out_t0 = (-b + sqrt_dis) / (2 * a);
	out_t1 = (-b - sqrt_dis) / (2 * a);

	if ((out_t0 < 0)  && (out_t1 < 0)) // if both roots are negative, they are behind camera
	{
		return false;
	}

	if ((out_t0 >= 0) && out_t1 >= 0) // if both are real, non-neg, pick smallest value
	{
		if (out_t0 < out_t1)
		{
			out_t = out_t0;
		}
		else if (out_t1 < out_t0)
		{
			out_t = out_t1;
		}
		else
		{
			out_t = out_t0;
		}
	}
	else if (((out_t0 >= 0) && (out_t1 < 0)) || ((out_t0 < 0) && (out_t1 >= 0)))
	{
		if (out_t0 > out_t1)
		{
			out_t = out_t0;
		}
		else
		{
			out_t = out_t1;
		}
	}
	return true;
	*/

	
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

glm::vec3 sphere::get_normal(glm::vec3 & intersect_point) const //TODO verify correct
{
	glm::vec4 obj_space_p = m_transform_stack_inv *(glm::vec4(intersect_point, 1.0f));
	float w_inv = 1 / obj_space_p.w;
	glm::vec4 dehom_obj_space_p = glm::vec4(obj_space_p.x * w_inv,
										  obj_space_p.y * w_inv,
										  obj_space_p.z * w_inv,
										  0.f);

	glm::vec4 obj_space_normal = dehom_obj_space_p - glm::vec4(center, 0.f); // TODO need to normalize?
	glm::vec4 world_space_normal = (glm::transpose(m_transform_stack_inv))*obj_space_normal;

	return glm::normalize(glm::vec3(world_space_normal.x, world_space_normal.y, world_space_normal.z));
}

triangle::triangle(glm::vec3 &v0_con, glm::vec3 &v1_con, glm::vec3 &v2_con)
{
	v0 = v0_con;
	v1 = v1_con;
	v2 = v2_con;

//	tri_normal = glm::cross((v1 - v0), (v2 - v0));
//	N = glm::normalize(glm::cross((v1 - v0), (v2 - v0))); // TODO check if unit norm needed
	normal = glm::cross((v1 - v0), (v2 - v0));

	prim_ambient = glm::vec3(.2f, .2f, .2f); // TODO default ambient
}

bool triangle::intersect(const ray ray_shot, float &out_t)
{
	
	glm::vec3 v0v1 = v1 - v0;
	glm::vec3 v0v2 = v2 - v0;
	glm::vec3 Norm = glm::cross(v0v1, v0v2);
	
	
	float NdotRayDir = glm::dot(Norm, ray_shot.ray_dir);
	if (NdotRayDir == 0) { return false; }

	/*
	if (fabs(NdotRayDir) < k_eps) // near zero
	{	return false;	} // ray and plane are parallel
	*/


	float d = glm::dot(Norm, v0);
	float NdotOrg = glm::dot(Norm, ray_shot.ray_origin);

	// compute t
	out_t = (d - NdotOrg) / NdotRayDir; // TODO subtract?
	if (out_t < 0) return false; // triangle is behind rray

	// compute intersection
	glm::vec3 P_inter = ray_shot.ray_origin + out_t * ray_shot.ray_dir;

	// Test whether hit point is in our out of triangle using Barycentric Coordinates
	float area_tri_v012 = glm::length(Norm) / 2.0f; // TODO calc normal?

	// Sub-triange V1 V2 P
	glm::vec3 normal_sub_tri = glm::cross((v2 - v1), (P_inter - v1));
	float area_tri_v1P2 = glm::length(normal_sub_tri) / 2.0f;
	if (glm::dot(normal_sub_tri, Norm) < 0) return false;

	// Sub-triange V0 V1 P
	normal_sub_tri = glm::cross((v1 - v0), (P_inter - v0));
	float area_tri_v01P = glm::length(normal_sub_tri) / 2.0f;
	if (glm::dot(normal_sub_tri, Norm) < 0) return false;

	// Sub-triange V2 V0 P
	normal_sub_tri = glm::cross((v0 - v2), (P_inter - v2));
	float area_tri_v20P = glm::length(normal_sub_tri) / 2.0f;
	if (glm::dot(normal_sub_tri, Norm) < 0) return false;

	return true;

/*	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Triangle Intersect Code Below Fails for Some reason when casting shadow ray
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
	*/
}

glm::vec3 triangle::get_normal (glm::vec3 & intersect_point) const// TODO verify correct
{
	glm::vec4 obj_space_normal = glm::normalize(glm::vec4(normal, 0.0f));
	glm::vec4 world_space_normal = (glm::transpose(m_transform_stack_inv))*obj_space_normal;
	return glm::normalize(world_space_normal);
}


/*
glm::vec3 triangle::get_normal(glm::vec3 & intersect_point) const
{
	glm::vec4 temp = m_transform_stack_inv * glm::vec4(intersect_point, 1.0f);
	glm::vec3 p = glm::vec3(temp.x/temp.w, temp.y / temp.w, temp.z / temp.w);

	
	glm::vec3 n = glm::cross(v1 - v0, v2 - v0);
	glm::vec3 tmp_nb = glm::cross(v2- p, v0 - p);
	glm::vec3 tmp_nc = glm::cross(v0 - p, v1 - p);

	float beta = glm::dot(n, tmp_nb) / glm::dot(n, n);
	float gamma = glm::dot(n, tmp_nc) / glm::dot(n, n);
	float alpha = 1.0 - beta - gamma;
	glm::vec3 ret = (tri_normal * alpha) + (tri_normal* beta) + (tri_normal* gamma);
	return glm::vec3(glm::vec4(ret, 0.0f) * glm::transpose(this->m_transform_stack_inv));
}
*/