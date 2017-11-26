#include "raytracer.h"

raytracer::raytracer()
{
}

raytracer::~raytracer()
{
}

bool raytracer::trace_ray_to_primitive(const ray &rayshot, const std::vector<std::unique_ptr<primitive>> &scene_primitives, float & out_tNear, const primitive *& out_primitive_hit)
{
	out_tNear = INFINITY;

	// iterate through all scene primitives
	std::vector<std::unique_ptr<primitive>>::const_iterator iterator = scene_primitives.begin();
	for (; iterator != scene_primitives.end(); ++iterator)
	{
		float out_temp_t = INFINITY;

		if ((*iterator)->intersect(rayshot, out_temp_t) && out_temp_t < out_tNear)
		{
			out_primitive_hit = iterator->get();
			out_tNear = out_temp_t;
		}
	}
	
	// return false if no primitive has been hit
	return (out_primitive_hit != nullptr);
}

glm::vec3 raytracer::compute_pixel_color(const ray & rayshot, const std::vector<std::unique_ptr<primitive>>& scene_primitives)
{
	glm::vec3 hit_color = glm::vec3(0);
	const primitive *primitive_hit = nullptr;
	float t; // hit scalar in ray equation;

	if (trace_ray_to_primitive(rayshot, scene_primitives, t, primitive_hit))
	{
		// TODO complete algorithm to determine pixel color
		hit_color[0] = 255;
		hit_color[1] = 1;
		hit_color[2] = 1;
// TODO remove		std::cout << "it's true yo\n";
	}


	return hit_color;
}

// TODO fragment shader
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