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

void print_array_of_vectors(std::vector<glm::vec3> vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		printvec3(vec[i]);
	}
}
// !!!!!!!!!!!!!!!!!!!!


int main(int argc, char *argv[])
{
	// Parse file to extract scene variables
	scene scene;
	scene.readfile(argv[1]);
	
	// Assign scene varibles
	int Width = scene.width, Height = scene.height, BitsPerPixel = 24;
	// Camera
	glm::vec3 LookFrom = scene.LookFrom;
	glm::vec3 LookAt = scene.LookAt;
	glm::vec3 UpVec = scene.UpVec;
	float fovy = scene.fovy;
	// Scene primitives
	std::vector<primitive*> primitives = scene.v_primitives;
	// Scene lights
	std::vector<light*> scene_lights = scene.v_scene_lights;




	// TODO remove testing ground
	// Print debugging area
	// !!!!!!!!!!!!!!!!!!!!!!!!!
	printvec3(scene.LookAt);
	std::cout << scene.width << " " << scene.height << "\n";
	std::cout << "Max Verts: " << scene.max_verts << "\n";
	std::cout << "Number of vertices: " << scene.v_vertices.size() << "\n";
	std::cout << "Number of primitives: " << primitives.size() << "\n";
	std::cout << "Number of lights: " << scene_lights.size() << "\n";
	std::cout << "K-eps: " << k_eps << "\n";
	
	
	if (!scene_lights.empty())
	{
		std::cout << "Number of lights " << scene_lights.size() << "\n";
		printf("Light Dir Vec\n");
		printvec3(scene_lights.back()->dir_pos);
		printf("Light Color Vec\n");
		printvec3(scene_lights.back()->color);
		std::cout << "Use attenuation? = " << scene_lights.back()->use_attenuation << "\n";
		printf("Attenuation\n");
	}
	printvec3(scene.attenuation);
	
	std::cout << "Primitive ambient\n";
	printvec3(primitives.back()->prim_ambient);

	// !!!!!!!!!!!!!
	// !!!!!!!!!!!!!

	// pixel array to input to FreeImage
	BYTE *pixel_array = new BYTE[3*Width*Height];
	glm::vec3 color_vec;

	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width;j++)
		{
			ray ray_through_pixel = camera::create_ray(LookFrom, LookAt, UpVec, fovy, Width, Height, i, j);

			//!!!!!!!!!!!!!!!!!!Testing ground

			color_vec = raytracer::compute_pixel_color(ray_through_pixel, primitives, scene_lights, scene.attenuation);
						
			//!!!!!!!!!!!!!!End testing ground

			int slot = 3 * ((Height-i - 1)*Width + j);
			*(pixel_array + 0 + slot) = color_vec[0];
			*(pixel_array + 1 + slot) = color_vec[1];
			*(pixel_array + 2 + slot) = color_vec[2];
		}
		int counter = i % 50;
		if (counter == 0) std::cout << "Tracing pixel row: " << i << "\n";
	}

	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixel_array, Width, Height, Width * 3, 24, 0xFF0000, 0xFF0000, 0xFF0000, false);
	FreeImage_Save(FIF_PNG, img, "filename.png", 0);
	
	// End Program Display Text
	std::cout << "\n********************" << "\n";
	std::cout << "Ray Tracing Complete" << "\n";
	std::cout << "\n********************" << "\n";
	FreeImage_DeInitialise();
	system("Pause");
}