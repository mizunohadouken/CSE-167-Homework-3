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




	// TODO remove testing ground
	// Print debugging area
	// !!!!!!!!!!!!!!!!!!!!!!!!!
	printvec3(scene.LookAt);
	std::cout << scene.width << " " << scene.height << "\n";
	std::cout << "Max Verts: " << scene.max_verts << "\n";
	std::cout << "Number of vertices: " << scene.v_vertices.size() << "\n";
	std::cout << "Number of trianlges: " << scene.v_primitives.size() << "\n";
	std::cout << "Number of primitives: " << primitives.size() << "\n";

	printf("last ambient:\n");
	printvec3(scene.ambient);
	std::cout << "K-eps: " << k_eps << "\n";
	
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