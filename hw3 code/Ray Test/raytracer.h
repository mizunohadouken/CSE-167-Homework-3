#pragma once
#include "camera.h"
#include <vector>
#include <memory>
#include "primitive.h"
#include "math.h"
#include <iostream>
#include <algorithm>
#include "scene.h"

class raytracer
{
public:
	raytracer();
	~raytracer();
	static bool trace_ray_to_primitive(const ray & rayshot, std::vector<primitive*> scene_primitives, float & out_tNear, const primitive *& out_primitive_hit);
	static glm::vec3 compute_pixel_color(const ray& rayshot, std::vector<primitive*>& scene_primitives, std::vector<light*>& scene_lights, glm::vec3& light_attenuation);
	static glm::vec3 Lambert_Phong(const glm::vec3 & direction, const glm::vec3 & normal, const glm::vec3 & halfvec, const glm::vec3 & mydiffuse, const glm::vec3 & myspecular, const float myshininess);
//	glm::vec3 shader_equations();

private:

};
