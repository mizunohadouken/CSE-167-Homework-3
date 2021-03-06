#pragma once
#include "glm.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stack>
#include <vector>
#include <memory>
#include "primitive.h"
#include "camera.h"

class light
{
public:
	light();
	~light();

	light(glm::vec3 dir_pos_con, glm::vec3 color_con, bool attenutation_con);// , glm::mat4 model_view);

	glm::vec3 dir_pos;
	glm::vec3 color;
//	glm::vec4 transf_dir_pos;
//	glm::vec4 transf_color;
	bool use_attenuation;
private:

};

class scene
{
public:
	// variables to parse

	// scene defaults
	std::string output_filename;
	float width, height;
	int max_verts;
	int max_depth;

	// camera variables
	glm::vec3 LookFrom, LookAt, UpVec;
	float fovy;

	// light variables
	glm::vec3 attenuation;
	std::vector<light*> v_scene_lights;

	// temp material variables
	glm::vec3 diffuse, specular, emission, ambient;
	float shininess;

	// geometry/primitives
	std::vector<std::unique_ptr<primitive>> v_primitives;
	std::vector<glm::vec3> v_vertices;

	// constants
	float bias = .01f; // TODO adjustable bias for shadows/rays surface

	// methods
	void readfile(const char* filename);
	bool readvals(std::stringstream & s, const int numvals, float * values);
	void rightmultiply(const glm::mat4 & M, std::stack<glm::mat4> &transfstack);
	void matransform(std::stack<glm::mat4> &transfstack, float* values);

	scene();
	~scene();

private:

};