#include "raytracer.h"

raytracer::raytracer()
{
}

raytracer::~raytracer()
{
}

bool raytracer::trace_ray_to_primitive(const ray & rayshot, const std::vector<std::unique_ptr<primitive>>& scene_primitives, float & out_tNear, const primitive *& out_primitive_hit, glm::vec3& out_prim_int)
{
	out_tNear = INFINITY;

	// iterate through all scene primitives
	std::vector<std::unique_ptr<primitive>>::const_iterator iterator = scene_primitives.begin();
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
				out_primitive_hit = iterator->get();
				out_prim_int = temp_prim_int;
				out_tNear = reverted_t;
			}
		}
	}
	
	// return false if no primitive has been hit
	return (out_primitive_hit != nullptr);
}

glm::vec3 raytracer::compute_pixel_color(const ray& rayshot, const std::vector<std::unique_ptr<primitive>>& scene_primitives, std::vector<light*>& scene_lights, scene& scene, int depth)
{
	glm::vec3 pixel_color_sum = glm::vec3(0.f);
	glm::vec3 out_prim_int;
	const primitive *out_primitive_hit = nullptr;
	float out_t;  // hit scalar in ray equation;
	float dist_to_light;

	if (trace_ray_to_primitive(rayshot, scene_primitives, out_t, out_primitive_hit, out_prim_int)) // returns t intersection, nearest primitive hit, and vec3 intersect point
	{
		// first add ambient and emission terms if ray hits a primitive
//		pixel_color_sum = pixel_color_sum + out_primitive_hit->prim_ambient + out_primitive_hit->prim_emission;
		pixel_color_sum = out_primitive_hit->prim_ambient + out_primitive_hit->prim_emission;

		// iterate through lights and add contribution to color
		for (unsigned int i = 0; i < scene_lights.size(); i++)
		{
			bool is_visible = true;
			glm::vec3 calc_atten = glm::vec3(0.f);
			glm::vec3 light_direction;

	//		if (((scene_lights[i]->use_attenuation)) == 1) // if true, point light
			if (((scene_lights[i]->use_attenuation)))
			{
//				light_direction = out_prim_int - scene_lights[i]->dir_pos; // TODO check order of subtraction
				light_direction = scene_lights[i]->dir_pos - out_prim_int;

				float r = glm::length(light_direction);
				dist_to_light = r;

				calc_atten = (scene_lights[i]->color) / (scene.attenuation.x + (scene.attenuation.y)*r + (scene.attenuation.z)*r*r); // TODO verify correct
				light_direction = glm::normalize(light_direction);

				// Shoot shadow ray from intersect point
	//			is_visible = trace_shadow(light_direction, out_prim_int, out_primitive_hit, scene_primitives, dist_to_light);
			}
//			else if ((scene_lights[i]->use_attenuation) == 0) // if false, directional light, no attenuation
			if (!(scene_lights[i]->use_attenuation)) // if false, directional light, no attenuation
			{
				light_direction = scene_lights[i]->dir_pos; // TODO Positive or negative? Normalize?


				calc_atten = scene_lights[i]->color;

				dist_to_light = INFINITY; // directional lights are placed in an infinite distance away
				light_direction = glm::normalize(light_direction);

				// Shoot shadow ray from intersect point
	//			is_visible = trace_shadow(light_direction, out_prim_int, out_primitive_hit, scene_primitives, dist_to_light);
			}

			is_visible = trace_shadow(light_direction, out_prim_int, scene_primitives, dist_to_light);

//			glm::vec3 L = -light_direction;
			glm::vec3 L = light_direction;
			glm::vec3 hit_normal = glm::normalize(out_primitive_hit->get_normal(out_prim_int));
			glm::vec3 eye_v = glm::normalize(rayshot.ray_origin - out_prim_int);

			// Calculate color at pixel using lambert and phong
			if (is_visible)
			{
		//		glm::vec3 half_vec = glm::normalize(L - rayshot.ray_dir); // TODO verify this is correct
				glm::vec3 half_vec = glm::normalize(L + eye_v); // TODO verify this is correct

				glm::vec3 lamb_phong = Lambert_Phong(L,
					hit_normal,
					half_vec,
					out_primitive_hit->prim_diffuse,
					out_primitive_hit->prim_specular,
					out_primitive_hit->prim_shininess);
				pixel_color_sum = pixel_color_sum + calc_atten*lamb_phong;
			}
			// end diffuse and specular contribution
		}
		glm::vec3 hit_normal = glm::normalize(out_primitive_hit->get_normal(out_prim_int));

		// Calculate reflective term, recursive tracing
		if (depth == scene.max_depth)
		{
			return glm::vec3(0.f, 0.f, 0.f);
		}
		if (depth < scene.max_depth)
		{
			glm::vec3 eyeVec = (rayshot.ray_origin - out_prim_int);

			//			glm::vec3 refl_ray_dir = glm::normalize(rayshot.ray_dir - 2.0f * glm::dot(rayshot.ray_dir, hit_normal) * hit_normal);
			glm::vec3 refl_ray_dir = glm::normalize(2.f * (glm::dot(eyeVec, hit_normal) * hit_normal) - eyeVec);
			ray refl_ray = ray(out_prim_int + scene.bias*refl_ray_dir, refl_ray_dir);

			//			glm::vec3 refl_color = .5f*(out_primitive_hit->prim_specular)*(compute_pixel_color(refl_ray, scene_primitives, scene_lights, scene, depth + 1));
			glm::vec3 refl_color = compute_pixel_color(refl_ray, scene_primitives, scene_lights, scene, depth + 1);
			pixel_color_sum = pixel_color_sum + (out_primitive_hit->prim_specular)*refl_color;
		}
		// end reflective contribution
	}
	return pixel_color_sum;
}

// TODO refactor later
/*
ray raytracer::reflect_ray(const ray& incident_ray, glm::vec3& surface_normal, glm::vec3& hit_point)
{
	float shadow_bias = .0001f;
	glm::vec3 int_to_eye = -incident_ray.ray_dir;
	glm::vec3 refl_dir = (2.0f * (glm::dot(int_to_eye, surface_normal)) * surface_normal) - int_to_eye; // TODO verify signs are correct
	refl_dir = glm::normalize(refl_dir);
	return ray(hit_point + shadow_bias*refl_dir,
			   refl_dir);
}
*/

glm::vec3 raytracer::Lambert_Phong(const glm::vec3& direction,
					   const glm::vec3& normal, const glm::vec3& halfvec,
					   const glm::vec3& mydiffuse, const glm::vec3 &myspecular, const float myshininess)
{
	float nDotL = dot(normal, direction);
	glm::vec3 lambert = mydiffuse * std::max(nDotL, 0.0f);
	
	float nDotH = dot(normal, halfvec);
	glm::vec3 phong = myspecular * std::pow(std::max(nDotH, 0.0f), myshininess);

	glm::vec3 retval = lambert + phong;
	return retval;
}

bool raytracer::trace_shadow(glm::vec3& light_dir, glm::vec3& prim_intersect, const std::vector<std::unique_ptr<primitive>>& scene_primitives, float& dist_to_light)
{
//	float shadow_bias = .001f;
	float shadow_bias = .01f;
//	glm::vec3 dir_to_light_source = -light_dir; // TODO verify? point and directional same?
	glm::vec3 dir_to_light_source = light_dir;
	ray shadow_ray(prim_intersect + shadow_bias * dir_to_light_source,// +shadow_bias*(glm::normalize(prim_hit->get_normal(prim_intersect))), // origin of ray at intersect point of primitive with light offset
					dir_to_light_source);																// direction of ray, towards light


	glm::vec3 temp_prim_int;
	const primitive* temp_prim = nullptr;
	float temp_out_t;
//	return !(trace_ray_to_primitive(shadow_ray, scene_primitives, temp_out_t, temp_prim, temp_prim_int)); // TODO change this back
//		trace_ray_to_primitive(rayshot, scene_primitives, out_t, out_primitive_hit, out_prim_int))
return !(check_blocking_light(shadow_ray, scene_primitives, dist_to_light));
}

bool raytracer::check_blocking_light(ray& shadow_ray, const std::vector<std::unique_ptr<primitive>>& scene_primitives, float& dist_to_light)
{
	const primitive *temp_prim = nullptr;

	// iterate through all scene primitives
	std::vector<std::unique_ptr<primitive>>::const_iterator iterator = scene_primitives.begin();
	glm::vec3 temp_prim_int;
	for (; iterator != scene_primitives.end(); ++iterator)
	{
		float out_temp_t = INFINITY;

		ray inv_ray = (*iterator)->inv_transform_ray(shadow_ray); // transform ray to object space of primitive
		if ((*iterator)->intersect(inv_ray, out_temp_t)) // check if ray intersects primitive
		{
			float reverted_t = (*iterator)->revert_t(inv_ray, out_temp_t, shadow_ray.ray_origin, temp_prim_int);

			if (reverted_t < dist_to_light)
			{
				temp_prim = iterator->get();
			}
		}
	}
	// return false if no primitive has been hit
	return temp_prim != nullptr;
}
