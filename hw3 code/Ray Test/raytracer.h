#pragma once
#include "camera.h"
#include <vector>
#include <memory>
#include "primitive.h"
#include "math.h"
#include <iostream>

class raytracer
{
public:
	raytracer();
	~raytracer();
	static bool trace_ray_to_primitive(const ray &rayshot, const std::vector<std::unique_ptr<primitive>> &scene_primitives, float &out_tNear, const primitive *&out_primitive_hit);
	static glm::vec3 compute_pixel_color(const ray &rayshot, const std::vector<std::unique_ptr<primitive>> &scene_primitives);
//	glm::vec3 shader_equations();

private:

};
