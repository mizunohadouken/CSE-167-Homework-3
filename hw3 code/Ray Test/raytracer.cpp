#include "raytracer.h"

raytracer::raytracer()
{
}

raytracer::~raytracer()
{
}

bool raytracer::trace_ray_to_primitive(const ray & rayshot, std::vector<primitive*> scene_primitives, float & out_tNear, const primitive *& out_primitive_hit)
{
	out_tNear = INFINITY;

	// iterate through all scene primitives
	std::vector<primitive*>::iterator iterator = scene_primitives.begin();

	for (; iterator != scene_primitives.end(); ++iterator)
	{
		float out_temp_t = INFINITY;
				
		ray inv_ray = (*iterator)->inv_transform_ray(rayshot);
		if ((*iterator)->intersect(inv_ray, out_temp_t))
		{
			float reverted_t = (*iterator)->revert_t(inv_ray, out_temp_t, rayshot.ray_origin);
			if (reverted_t < out_tNear)
			{
				out_primitive_hit = *iterator;
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
	glm::vec3 prim_intersection;
	glm::vec3 half_vec;
	glm::vec3 light_sum = glm::vec3(0);
	glm::vec3 hit_color = glm::vec3(0);
	const primitive *out_primitive_hit = nullptr;
	const primitive *temp_prim = nullptr;
	float out_t;  // hit scalar in ray equation;
	float temp_t;

	if (trace_ray_to_primitive(rayshot, scene_primitives, out_t, out_primitive_hit))
	{
		prim_intersection = rayshot.ray_origin + rayshot.ray_dir * out_t; // TODO Check this
		
		for (int i = 0; i < scene_lights.size(); i++)
		{
			bool is_visible = true;
			glm::vec3 calc_atten = glm::vec3(0);
			glm::vec3 light_direction = glm::vec3(0);

			if (((scene_lights[i]->use_attenuation)) == 1) // if true, point light
			{
				float light_denom = 1 / scene_lights[i]->dir_pos.w;
				glm::vec3 dehom_light_pos = scene_lights[i]->dir_pos*light_denom;

				light_direction = glm::normalize(prim_intersection - (dehom_light_pos)); // TODO check order of subtraction
				float r = glm::length(light_direction);  // TODO check calculation for magnitude, may be reversed
				light_direction.x = light_direction.x / r;
				light_direction.y = light_direction.y / r;
				light_direction.z = light_direction.z / r;

				calc_atten = scene_lights[i]->color;
			//	calc_atten = glm::normalize(scene_lights[i]->color);// / (light_attenuation.x + (light_attenuation.y)*r + (light_attenuation.z)*r*r);
			}
			else if ((scene_lights[i]->use_attenuation) == 0) // if false, directional light, no attenuation
			{
				light_direction = glm::normalize(scene_lights[i]->dir_pos); // TODO Positive or negative? Normalize?
				calc_atten = scene_lights[i]->color;
			}

/*
			// CHECK !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// Shadow !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			glm::vec3 to_light_source = -glm::normalize(light_direction); // TODO verify? point and directional same?
			ray r_inter_to_light(prim_intersection + .01f*(glm::normalize(out_primitive_hit->get_normal(prim_intersection))), // TODO offset? move ray origin small distance towards light
								to_light_source);

			is_visible = !(trace_ray_to_primitive(r_inter_to_light, scene_primitives, temp_t, temp_prim));
			// END SHADOW !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

			// Calculate color at pixel using lambert and phong
			glm::vec3 L = -light_direction;

			if (is_visible)
			{
				half_vec = glm::normalize(light_direction + glm::normalize(rayshot.ray_origin)); // TODO verify this is correct
				glm::vec3 hit_normal = glm::normalize(out_primitive_hit->get_normal(prim_intersection));

				glm::vec3 lamb_phong = Lambert_Phong(L,
					hit_normal,
					half_vec,
					out_primitive_hit->prim_diffuse,
					out_primitive_hit->prim_specular,
					out_primitive_hit->prim_shininess);

				light_sum = light_sum + calc_atten*lamb_phong;
			}
			else if (!is_visible)
			{

			}
		}

		glm::vec3 calc_color = out_primitive_hit->prim_ambient + out_primitive_hit->prim_emission + light_sum;
		hit_color[0] = 255 * (std::max(0.0f, std::min(1.0f, calc_color.b))); // clamp between 0 and 1
		hit_color[1] = 255 * (std::max(0.0f, std::min(1.0f, calc_color.g)));
		hit_color[2] = 255 * (std::max(0.0f, std::min(1.0f, calc_color.r)));
	}

	return hit_color;
}

// TODO pixel shader
glm::vec3 raytracer::Lambert_Phong(const glm::vec3& direction,
					   const glm::vec3& normal, const glm::vec3& halfvec,
					   const glm::vec3& mydiffuse, const glm::vec3 &myspecular, const float myshininess)
{
	float nDotL = dot(normal, direction);
	glm::vec3 lambert = mydiffuse * std::max(nDotL, 0.0f);
	
//	float nDotH = dot(normal, halfvec);
//	glm::vec3 phong = myspecular * pow(std::max(nDotH, 0.0f), myshininess);

	glm::vec3 retval = lambert;// +phong;
	return retval;
}


/*
glm::vec3 raytracer::shader_equations()
{
		glm::vec4 finalcolor;

		const vec3 eyepos = vec3(0, 0, 0);

		vec4 mv_pos = modelview * myvertex;
		vec3 mypos = mv_pos.xyz / mv_pos.w; // Dehomogenize current location 
		vec3 eyedirn = normalize(eyepos - mypos);

		// Compute normal, needed for shading. 
		vec3 mv_normal = mat3(transpose(inverse(modelview))) * mynormal;
		vec3 normal = normalize(mv_normal);

		// variables to store temps
		float light_i_nDotL, light_i_nDotH;
		vec3 dehomo_light_i_pos, light_i_direction, light_i_halfvec, light_input_dir;
		vec4 light_i_position, light_i_color, lambert, phong;
		vec4 phong_lambert_sum = vec4(0, 0, 0, 0);

		/////////////////////////////////
		// Loop to compute sum of lighting components
		for (int i = 0; i < numLights; i++)
		{
			light_i_position = lightposn[i];
			light_i_color = lightcolor[i];

			// for directional light
			if (light_i_position.w == 0)
			{
				light_input_dir = normalize(light_i_position.xyz);
			}
			// for point light
			else
			{
				dehomo_light_i_pos = light_i_position.xyz / light_i_position.w;
				light_input_dir = normalize(dehomo_light_i_pos - mypos);
			}

			light_i_halfvec = normalize(light_input_dir + eyedirn);

			phong_lambert_sum = phong_lambert_sum +
				ComputeLight(light_input_dir, light_i_color,
					normal, light_i_halfvec, diffuse,
					specular, shininess);
		}


		// Color all pixels black for now, remove this in your implementation!
		// finalcolor = vec4(0.0f, 0.0f, 0.0f, 1.0f); 
		finalcolor = ambient + emission + phong_lambert_sum;


		fragColor = finalcolor;
	}
}
*/