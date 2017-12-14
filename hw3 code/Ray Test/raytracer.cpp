#include "raytracer.h"

raytracer::raytracer()
{
}

raytracer::~raytracer()
{
}

bool raytracer::trace_ray_to_primitive(const ray & rayshot, std::vector<primitive*> scene_primitives, float & out_tNear, const primitive *& out_primitive_hit, glm::vec3& out_prim_int)
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

glm::vec3 raytracer::compute_pixel_color(const ray& rayshot, std::vector<primitive*>& scene_primitives, std::vector<light*>& scene_lights, glm::vec3& light_attenuation)
{
	glm::vec3 temp_vec;
	glm::vec3 half_vec;
	glm::vec3 light_sum = glm::vec3(0);
	glm::vec3 hit_color = glm::vec3(0);
	glm::vec3 out_prim_int;
	glm::vec3 temp_out_prim_int;
	const primitive *out_primitive_hit = nullptr;
	const primitive *temp_prim = nullptr;
	float out_t;  // hit scalar in ray equation;
	float temp_t;

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
			float shadow_bias = .0001f;
			glm::vec3 dir_to_light_source = -glm::normalize(light_direction); // TODO verify? point and directional same?
			ray shadow_ray(out_prim_int + shadow_bias*(glm::normalize(out_primitive_hit->get_normal(out_prim_int))), 
								dir_to_light_source);

			is_visible = !(trace_ray_to_primitive(shadow_ray, scene_primitives, temp_t, temp_prim, temp_out_prim_int));
			// END SHADOW !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


			// Calculate color at pixel using lambert and phong
			glm::vec3 L = -light_direction;

			if (is_visible)
			{
				half_vec = glm::normalize(light_direction + glm::normalize(rayshot.ray_origin)); // TODO verify this is correct
				glm::vec3 hit_normal = glm::normalize(out_primitive_hit->get_normal(out_prim_int));

				glm::vec3 lamb_phong = Lambert_Phong(L,
													 hit_normal,
													 half_vec,
													 out_primitive_hit->prim_diffuse,
													 out_primitive_hit->prim_specular,
													 out_primitive_hit->prim_shininess);
				light_sum = light_sum + calc_atten*lamb_phong;
			}
		}

		glm::vec3 calc_color = out_primitive_hit->prim_ambient + out_primitive_hit->prim_emission + light_sum;
		hit_color[0] = 255 * (std::max(0.0f, std::min(1.0f, calc_color.b))); // clamp between 0 and 1
		hit_color[1] = 255 * (std::max(0.0f, std::min(1.0f, calc_color.g)));
		hit_color[2] = 255 * (std::max(0.0f, std::min(1.0f, calc_color.r)));
	}

	return hit_color;
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