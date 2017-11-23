#include "raytracer.h"

raytracer::raytracer()
{
}

raytracer::~raytracer()
{
}

bool raytracer::trace_ray(const ray rayshot, const std::vector<std::unique_ptr<primitive>> scene_primitives, float & out_tNear, const primitive *& out_primitive_hit)
{
	float out_tNear = INFINITY;

	// iterate through all scene primitives
	std::vector<std::unique_ptr<primitive>>::const_iterator iterator = scene_primitives.begin();
	for (; iterator != scene_primitives.end(); ++iterator)
	{
		float out_temp_t = INFINITY;
		if ((*iterator)->intersect(rayshot, out_temp_t))
		{
			out_primitive_hit = iterator->get();
			out_tNear = out_temp_t;
		}
	}
	
	// return false if no primitive has been hit
	return (out_primitive_hit != nullptr);
}
