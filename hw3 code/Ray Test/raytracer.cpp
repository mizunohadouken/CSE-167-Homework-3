#include "raytracer.h"

raytracer::raytracer()
{
}

raytracer::~raytracer()
{
}

bool raytracer::trace_ray_to_primitive(const ray & rayshot, std::vector<primitive*>& scene_primitives, float & out_tNear, const primitive *& out_primitive_hit, glm::vec3& out_prim_int)
{
	out_tNear = INFINITY;

	// iterate through all scene primitives
	std::vector<primitive*>::iterator iterator = scene_primitives.begin();
	glm::vec3 temp_prim_int;

	for (; iterator != scene_primitives.end(); ++iterator)
	{
		float out_temp_t = INFINITY;
				
		ray inv_ray = (*iterator)->inv_transform_ray(rayshot); // transform ray to object space of primitive
		if ((*iterator)->intersect(inv_ray, out_temp_t)) // check if ray intersects primitive
		{
			float reverted_t = (*iterator)->revert_t(inv_ray, out_temp_t, rayshot.ray_origin, temp_prim_int);
			if (reverted_t < out_tNear)
			{
				out_primitive_hit = *iterator;
				out_prim_int = temp_prim_int;
				out_tNear = reverted_t;
			}
		}
	}
	
	// return false if no primitive has been hit
	return (out_primitive_hit != nullptr);
}

glm::vec3 raytracer::compute_pixel_color(const ray& rayshot, std::vector<primitive*>& scene_primitives, std::vector<light*>& scene_lights, glm::vec3& light_attenuation, int& max_depth, int depth)
{
	if(depth > max_depth)
	{
		glm::vec3 black = glm::vec3(0);
		return black;
	}

	glm::vec3 hit_color;// = glm::vec3(0);
	glm::vec3 refl_color;
	glm::vec3 temp_vec;
	glm::vec3 half_vec;
	glm::vec3 diffuse_specular_sum = glm::vec3(0);
	glm::vec3 out_prim_int;
	glm::vec3 temp_out_prim_int;
	const primitive *out_primitive_hit = nullptr;
	const primitive *temp_prim = nullptr;
	float out_t;  // hit scalar in ray equation;
	float temp_t;
	float bias = .0001f;

	if (trace_ray_to_primitive(rayshot, scene_primitives, out_t, out_primitive_hit, out_prim_int)) // returns t intersection, nearest primitive hit, and vec3 intersect point
	{
		for (int i = 0; i < scene_lights.size(); i++)
		{
			bool is_visible = true;
			glm::vec3 calc_atten = glm::vec3(0);
			glm::vec3 light_direction = glm::vec3(0);

			if (((scene_lights[i]->use_attenuation)) == 1) // if true, point light
			{
				light_direction = glm::normalize(out_prim_int - scene_lights[i]->dir_pos); // TODO check order of subtraction
				float r = glm::length(light_direction);

				calc_atten = (scene_lights[i]->color) / (light_attenuation.x + (light_attenuation.y)*r + (light_attenuation.z)*r*r); // TODO verify correct
			}
			else if ((scene_lights[i]->use_attenuation) == 0) // if false, directional light, no attenuation
			{
				light_direction = -glm::normalize(scene_lights[i]->dir_pos); // TODO Positive or negative? Normalize?
				calc_atten = scene_lights[i]->color;
			}
	
			// Shoot shadow ray from intersect point
			is_visible = trace_shadow(light_direction, out_prim_int, out_primitive_hit, scene_primitives);

			glm::vec3 L = -light_direction;
			glm::vec3 hit_normal = glm::normalize(out_primitive_hit->get_normal(out_prim_int));

			// Calculate color at pixel using lambert and phong
			if (is_visible)
			{
				half_vec = glm::normalize(L - glm::normalize(rayshot.ray_dir)); // TODO verify this is correct

				glm::vec3 lamb_phong = Lambert_Phong(L,
													 hit_normal,
													 half_vec,
													 out_primitive_hit->prim_diffuse,
													 out_primitive_hit->prim_specular,
													 out_primitive_hit->prim_shininess);
				diffuse_specular_sum = diffuse_specular_sum + calc_atten*lamb_phong;
			}

			// CALCULATE SPECULAR
//			ray refl_ray = reflect_ray(rayshot, hit_normal, out_prim_int);
			glm::vec3 refl_ray_dir = rayshot.ray_dir - 2.0f * glm::dot(rayshot.ray_dir, hit_normal) * hit_normal;
			glm::vec3 refl_ray_dir_norm = glm::normalize(refl_ray_dir);
			ray refl_ray = ray(out_prim_int + bias*hit_normal, refl_ray_dir_norm);
			refl_color = compute_pixel_color(refl_ray, scene_primitives, scene_lights, light_attenuation, max_depth, depth+1);
		}
		
		glm::vec3 calc_color = out_primitive_hit->prim_ambient
								+ out_primitive_hit->prim_emission
								+ diffuse_specular_sum
								+ (out_primitive_hit->prim_specular)*(refl_color);



		hit_color[0] = 255 * (std::max(0.0f, std::min(1.0f, calc_color.b))); // clamp between 0 and 1
		hit_color[1] = 255 * (std::max(0.0f, std::min(1.0f, calc_color.g)));
		hit_color[2] = 255 * (std::max(0.0f, std::min(1.0f, calc_color.r)));
	}

	return hit_color;
}

ray raytracer::reflect_ray(const ray& incident_ray, glm::vec3& surface_normal, glm::vec3& hit_point)
{
	float shadow_bias = .0001f;
	glm::vec3 int_to_eye = -incident_ray.ray_dir;
	glm::vec3 refl_dir = (2.0f * (glm::dot(int_to_eye, surface_normal)) * surface_normal) - int_to_eye; // TODO verify signs are correct
	refl_dir = glm::normalize(refl_dir);
	return ray(hit_point + shadow_bias*refl_dir,
			   refl_dir);
}

glm::vec3 raytracer::Lambert_Phong(const glm::vec3& direction,
					   const glm::vec3& normal, const glm::vec3& halfvec,
					   const glm::vec3& mydiffuse, const glm::vec3 &myspecular, const float myshininess)
{
	float nDotL = dot(normal, direction);
	glm::vec3 lambert = mydiffuse * std::max(nDotL, 0.0f);
	
	float nDotH = dot(normal, halfvec);
	glm::vec3 phong = myspecular * pow(std::max(nDotH, 0.0f), myshininess);

	glm::vec3 retval = lambert + phong;
	return retval;
}

bool raytracer::trace_shadow(glm::vec3 light_dir, glm::vec3 prim_intersect, const primitive *& prim_hit, std::vector<primitive*>& scene_primitives)
{
	glm::vec3 temp_out_prim_int;
	const primitive *temp_prim = nullptr;
	float temp_t;
	bool is_visible;
	
	float shadow_bias = .0001f;
	glm::vec3 dir_to_light_source = -glm::normalize(light_dir); // TODO verify? point and directional same?
	ray shadow_ray(prim_intersect + shadow_bias * dir_to_light_source, // + shadow_bias*(glm::normalize(prim_hit->get_normal(prim_intersect))), // origin of ray at intersect point of primitive with light offset
					dir_to_light_source);																// direction of ray, towards light

	return is_visible = !(trace_ray_to_primitive(shadow_ray, scene_primitives, temp_t, temp_prim, temp_out_prim_int));
}
