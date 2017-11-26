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



// TODO parse file 
class scene
{
public:
	// variables to parse
	float width;
	float height;
	glm::vec3 LookFrom;
	glm::vec3 LookAt;
	glm::vec3 UpVec;
	float fovy;
	std::vector<primitive*> v_primitives;
	// TODO,  add lighting variables

	// methods
	void readfile(const char* filename);
	bool readvals(std::stringstream & s, const int numvals, float * values);
	scene();
	~scene();

private:

};