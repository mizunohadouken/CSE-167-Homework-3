#pragma once

#include "glm.hpp"
#include <iostream>


void print_array(float *array_to_print)
{
	for (int i = 0; i < sizeof(array_to_print); i++)
	{
		std::cout << "Value " << i << ":" << array_to_print[i] << "\n";
	}
}

void printvec3(glm::vec3 vPrintMe)
{
	std::cout << "Vector x: " << vPrintMe.x << "\n";
	std::cout << "Vector y: " << vPrintMe.y << "\n";
	std::cout << "Vector z: " << vPrintMe.z << "\n";

}


void print3x3Matrix(glm::mat3 mPrintMe)
{
	std::cout << mPrintMe[0][0] << " ";
	std::cout << mPrintMe[1][0] << " ";
	std::cout << mPrintMe[2][0] << "\n";
	std::cout << mPrintMe[0][1] << " ";
	std::cout << mPrintMe[1][1] << " ";
	std::cout << mPrintMe[2][1] << "\n";
	std::cout << mPrintMe[0][2] << " ";
	std::cout << mPrintMe[1][2] << " ";
	std::cout << mPrintMe[2][2] << " ";

	return;
}

void print4x4Matrix(glm::mat4 mPrintMe)
{
	std::cout << mPrintMe[0][0] << " ";
	std::cout << mPrintMe[1][0] << " ";
	std::cout << mPrintMe[2][0] << " ";
	std::cout << mPrintMe[3][0] << "\n";
	std::cout << mPrintMe[0][1] << " ";
	std::cout << mPrintMe[1][1] << " ";
	std::cout << mPrintMe[2][1] << " ";
	std::cout << mPrintMe[3][1] << "\n";
	std::cout << mPrintMe[0][2] << " ";
	std::cout << mPrintMe[1][2] << " ";
	std::cout << mPrintMe[2][2] << " ";
	std::cout << mPrintMe[3][2] << "\n";
	std::cout << mPrintMe[0][3] << " ";
	std::cout << mPrintMe[1][3] << " ";
	std::cout << mPrintMe[2][3] << " ";
	std::cout << mPrintMe[3][3] << "\n";
	return;
}

void print_float(float x) { std::cout << "Value: " << x << "\n"; }

void test_line(std::string section_string)
{
	std::cout << "made it to: " << section_string << "\n";
}
