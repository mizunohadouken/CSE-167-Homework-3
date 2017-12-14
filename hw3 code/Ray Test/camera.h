#pragma once
#include "glm.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

struct ray
{
	glm::vec3 ray_origin;
	glm::vec3 ray_dir;
	ray();
	ray(glm::vec3 ray_origin_con, glm::vec3 ray_dir_con);
};

class camera
{

public:
	camera();
	virtual ~camera();
	static glm::mat4 lookAt(const glm::vec3& eye, const glm::vec3 &center, const glm::vec3& up);
	static glm::mat4 make_coordinate_frame(const glm::vec3& eye, const glm::vec3 &center, const glm::vec3& up);
	static glm::mat4 perspective(float fovy, float aspect, float zNear, float zFar);
	static glm::mat3 rotate(const float degrees, const glm::vec3& axis);
	static glm::mat4 scale(const float &sx, const float &sy, const float &sz);
	static glm::mat4 translate(const float &tx, const float &ty, const float &tz);
	static glm::vec3 upvector(const glm::vec3 &up, const glm::vec3 &zvec);
	static ray create_ray(glm::vec3 eye, glm::vec3 center, glm::vec3 up, float fovy, int width, int height, int i_pixel, int j_pixel);
};
