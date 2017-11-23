#include <iostream>
#include "glm.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "FreeImage.h"
#include <GLFW/glfw3.h>

#include "camera.h"
#include "primitive.h"
#include <algorithm>


// TODO Remove this test code -------------
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

void printvec3(glm::vec3 vPrintMe)
{
	std::cout << "Vector x: " << vPrintMe.x << "\n";
	std::cout << "Vector y: " << vPrintMe.y << "\n";
	std::cout << "Vector z: " << vPrintMe.z << "\n";

}

void print_float(float x) { std::cout << "Value: " << x << "\n"; }
// End Test Code
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



int main(int argc, char *argv[])
{
	// TODO change variables based on parser (scene)!!!!!!!!!!!!!!
	// Test Scene 1 Variables !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Size
	const int Width = 640, Height = 480, BitsPerPixel = 24;
	// Camera
	glm::vec3 LookFrom = glm::vec3(0, 0, 4);
	glm::vec3 LookAt = glm::vec3(0, 0, 0);
	glm::vec3 UpVec = glm::vec3(0, 1, 0);
	float fovy = 30;
	// other
	glm::vec3 ambient = glm::vec3(.1, .1, .1);
	glm::vec3 diffuse = glm::vec3(1, 0, 0);
	glm::vec3 specular = glm::vec3(0, 0, 0);
	// lights
	glm::vec3 point_light_dir = glm::vec3(4, 0, 4);
	glm::vec3 point_light_color = glm::vec3(.5, .5, .5);
	glm::vec3 directional_light_dir = glm::vec3(0, 0, 1);
	glm::vec3 directional_light_color = glm::vec3(.5, .5, .5);
	// geometry
	int maxverts = 4;
	glm::vec3 vertex1 = glm::vec3(-1, -1, 0);
	glm::vec3 vertex2 = glm::vec3(1, -1, 0);
	glm::vec3 vertex3 = glm::vec3(1, 1, 0);
	glm::vec3 vertex4 = glm::vec3(-1, 1, 0);
	glm::vec3 triangle1 = glm::vec3(0, 1, 2);
	glm::vec3 triangle2 = glm::vec3(0, 2, 3);
	// End Test Scene 1 Variables !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	BYTE temp_arr[3*Width*Height];
	BYTE *pixel_array = temp_arr;//new BYTE[3*Width*Height];
	camera scene_cam;


	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width;j++)
		{
			ray ray_through_pixel = scene_cam.create_ray(LookFrom, LookAt, UpVec, fovy, Width, Height, i, j);




			int slot = 3 * ((Height - i - 1)*Width + j);
			*(pixel_array + 0 + slot) = i*.255;
			*(pixel_array + 1 + slot) = i*.955;
			*(pixel_array + 2 + slot) = i*.555;
		}
	}

	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixel_array, Width, Height, Width * 3, 24, 0xFF0000, 0xFF0000, 0xFF0000, false);
	FreeImage_Save(FIF_PNG, img, "filename.png", 0);

//!!!!!!!!!!!!!!!!!!Testing ground
	float a = 2, b = -8, c = -24, x0, x1;
	glm::vec3 test_vec = glm::vec3(1, 2, 3);
	glm::vec3 test_vec2 = glm::vec3(4, 2, 3);

	sphere test_sphere(test_vec, 2);
	print_float(test_sphere.get_radius());

	//!!!!!!!!!!!!!!End testing ground
	


	// Display Text
	std::cout << "Free Image_" << FreeImage_GetVersion() << "\n";
	std::cout << FreeImage_GetCopyrightMessage() << "\n\n";
	FreeImage_DeInitialise();
	system("Pause");
}