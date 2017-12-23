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
	static bool trace_ray_to_primitive(const ray & rayshot, std::vector<primitive*>& scene_primitives, float & out_tNear, const primitive *& out_primitive_hit, glm::vec3& out_prim_int);
	static glm::vec3 compute_pixel_color(const ray& rayshot, std::vector<primitive*>& scene_primitives, std::vector<light*>& scene_lights, glm::vec3& light_attenuation, int& max_depth, int depth);
	static ray reflect_ray(const ray& incident_ray, glm::vec3& surface_normal, glm::vec3& hit_point);
	static glm::vec3 Lambert_Phong(const glm::vec3 & direction, const glm::vec3 & normal, const glm::vec3 & halfvec, const glm::vec3 & mydiffuse, const glm::vec3 & myspecular, const float myshininess);
	static bool trace_shadow(glm::vec3 light_dir, glm::vec3 prim_intersect, const primitive *& prim_hit, std::vector<primitive*>& scene_primitives, float& dist_to_light);

	static bool check_blocking_light(ray & shadow_ray, std::vector<primitive*>& scene_primitives, float & dist_to_light);

private:

};
