#include <iostream>
#include "glm.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "FreeImage.h"
#include <GLFW/glfw3.h>

#include "scene.h"
#include "camera.h"
#include "primitive.h"
#include "raytracer.h"
#include <algorithm>


// TODO remove
// !!!!!!!!!!!!!

void test_line(std::string section_string)
{
	std::cout << "made it to: " << section_string << "\n";
}


void printvec3(glm::vec3 vPrintMe)
{
	std::cout << "Vector x: " << vPrintMe.x << "\n";
	std::cout << "Vector y: " << vPrintMe.y << "\n";
	std::cout << "Vector z: " << vPrintMe.z << "\n";

}
// !!!!!!!!!!!!!!!!!!!!


int main(int argc, char *argv[])
{


	// TODO change variables based on parser (scene)!!!!!!!!!!!!!!
	// Test Scene 1 Variables !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Size
	const int Width = 640, Height = 480, BitsPerPixel = 24;
	// Camera
	glm::vec3 LookFrom = glm::vec3(-2, -2, 2);
	glm::vec3 LookAt = glm::vec3(0, 0, 0);
	glm::vec3 UpVec = glm::vec3(1, 1, 2);
	float fovy = 60;
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

	scene scene;
	scene.readfile(argv[1]);

	// TODO remove testing ground
	// !!!!!!!!!!!!!!!!!!!!!!!!!
	printvec3(scene.LookAt);
	std::cout << scene.width << " " << scene.height;

//	scene.v_primitives->
	// !!!!!!!!!!!!!
	// !!!!!!!!!!!!!


	BYTE temp_arr[3*Width*Height];
	BYTE *pixel_array = temp_arr;//new BYTE[3*Width*Height];
	camera scene_cam;


	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width;j++)
		{
			ray ray_through_pixel = scene_cam.create_ray(LookFrom, LookAt, UpVec, fovy, Width, Height, i, j);

			//!!!!!!!!!!!!!!!!!!Testing ground
			glm::vec3 sphere_test_vec = glm::vec3(1.f, -0.5f, -0.5f);
			glm::vec3 sphere_test_vec2 = glm::vec3(-1.f, -0.5f, -0.5f);
			float sphere_test_radius = .15f;

			std::vector<primitive*> primitives;
			primitives = scene.v_primitives;
			glm::vec3 color_vec;
			color_vec = raytracer::compute_pixel_color(ray_through_pixel, primitives);

			
			//!!!!!!!!!!!!!!End testing ground

			int slot = 3 * ((Height - i - 1)*Width + j);
			*(pixel_array + 0 + slot) = color_vec[0];
			*(pixel_array + 1 + slot) = color_vec[1];
			*(pixel_array + 2 + slot) = color_vec[2];

		}
	}


	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixel_array, Width, Height, Width * 3, 24, 0xFF0000, 0xFF0000, 0xFF0000, false);
	FreeImage_Save(FIF_PNG, img, "filename.png", 0);


	


	// Display Text
	std::cout << "Free Image_" << FreeImage_GetVersion() << "\n";
	std::cout << FreeImage_GetCopyrightMessage() << "\n\n";
	FreeImage_DeInitialise();
	system("Pause");
}