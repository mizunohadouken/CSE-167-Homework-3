#pragma once
#include "camera.h"
#include <vector>
#include <memory>
#include "primitive.h"
#include "math.h"

class raytracer
{
public:
	raytracer();
	~raytracer();
	static bool trace_ray(const ray rayshot, const std::vector<std::unique_ptr<primitive>> scene_primitives, float &out_tNear, const primitive *&out_primitive_hit);

private:

};
