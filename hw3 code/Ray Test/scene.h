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
// #include "print_helpers.h"

class scene
{
public:
	// variables to parse

	// scene defaults
	float width, height;
	int max_verts;

	// camera variables
	glm::vec3 LookFrom, LookAt, UpVec;
	float fovy;

	// light variables
//	glm::vec3 ;

	// temp material variables
	glm::vec3 diffuse, specular, emission, ambient;
	float shininess;


	// geometry/primitives
	std::vector<primitive*> v_primitives;
	std::vector<glm::vec3> v_vertices;

	// methods
	void readfile(const char* filename);
	bool readvals(std::stringstream & s, const int numvals, float * values);
	scene();
	~scene();

private:

};