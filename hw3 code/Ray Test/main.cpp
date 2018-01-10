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
	scene.output_filename = "default.png";
	scene.readfile(argv[1]);
	
	// Assign scene varibles
	int Width = scene.width, Height = scene.height, BitsPerPixel = 24;
	// Camera
	glm::vec3 LookFrom = scene.LookFrom;
	glm::vec3 LookAt = scene.LookAt;
	glm::vec3 UpVec = scene.UpVec;
	float fovy = scene.fovy;
	// Scene primitives
//	std::vector<std::unique_ptr<primitive>> primitives = scene.v_primitives;
	// Scene lights
	std::vector<light*> scene_lights = scene.v_scene_lights;



	// TODO remove testing ground
	// Print debugging area
	// !!!!!!!!!!!!!!!!!!!!!!!!!

	printf("Look at vector:\n");
	printvec3(scene.LookAt);/*
	std::cout << scene.width << " " << scene.height << "\n";
	std::cout << "Max Verts: " << scene.max_verts << "\n";
	std::cout << "Number of vertices: " << scene.v_vertices.size() << "\n";
	*/
	std::cout << "Number of primitives: " << scene.v_primitives.size() << "\n";
	std::cout << "Number of lights: " << scene_lights.size() << "\n";
	std::cout << "K-eps: " << k_eps << "\n";
	std::cout << "Max Depth: " << scene.max_depth << "\n\n";

	if (!scene_lights.empty())
	{
		printf("First light dir Vec\n");
		printvec3(scene_lights.front()->dir_pos);
		printf("First light color Vec\n");
		printvec3(scene_lights.front()->color);
		std::cout << "Use attenuation for first light? = " << scene_lights.front()->use_attenuation << "\n";
		printf("Last light dir Vec\n");
		printvec3(scene_lights.back()->dir_pos);
		printf("Last light color Vec\n");
		printvec3(scene_lights.back()->color);
		std::cout << "Use attenuation for last light? = " << scene_lights.back()->use_attenuation << "\n";
		printf("Attenuation\n");
		printvec3(scene.attenuation);
	}
	/*
	std::cout << "Last Primitive ambient\n";
	printvec3(primitives.back()->prim_ambient);
	std::cout << "Last Primitive specular\n";
	printvec3(primitives.back()->prim_specular);
	std::cout << "Last Primitive diffuse\n";
	printvec3(primitives.back()->prim_diffuse);
	std::cout << "Last Primitive emission\n";
	printvec3(primitives.back()->prim_emission);
	std::cout << "Last Primitive shiness: " << primitives.back()->prim_shininess << "\n";
	*/

	
	// !!!!!!!!!!!!!
	// !!!!!!!!!!!!!


	// pixel array to input to FreeImage
	BYTE *pixel_array = new BYTE[3*Width*Height];
	glm::vec3 color_vec;
	int temp;
	for (int i = 0.f; i < Height; i++)
	{
		for (int j = 0.f; j < Width;j++)
		{
			ray ray_through_pixel = camera::create_ray(LookFrom, LookAt, UpVec, fovy, Width, Height, i, j);
			color_vec = raytracer::compute_pixel_color(ray_through_pixel, scene.v_primitives, scene_lights, scene, 0);


			int slot = 3 * ((Height-i - 1)*Width + j); // starting at bottom left pixel
//			int slot = 3 * ((Width*i)+j);  // start at top right
/*
			// insert computed color into array for image
			*(pixel_array + 2 + slot) = 255.f * (std::max(0.0f, std::min(1.0f, color_vec.r))); // clamp between 0 and 1
			*(pixel_array + 1 + slot) = 255.f * (std::max(0.0f, std::min(1.0f, color_vec.g)));
			*(pixel_array + 0 + slot) = 255.f * (std::max(0.0f, std::min(1.0f, color_vec.b)));
			*/
			// insert computed color into array for image
			*(pixel_array + 2 + slot) = static_cast<unsigned int>(std::min(255 * color_vec.r, 255.0f)); // clamp between 0 and 1
			*(pixel_array + 1 + slot) = static_cast<unsigned int>(std::min(255 * color_vec.g, 255.0f));
			*(pixel_array + 0 + slot) = static_cast<unsigned int>(std::min(255 * color_vec.b, 255.0f));
		}
		temp = i;
		int counter = temp %10;
		if (counter == 0) std::cout << "Tracing pixel row: " << i << "\n";
	}
	
	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixel_array, Width, Height, Width * 3, 24, 0xFF0000, 0xFF0000, 0xFF0000, false);
	FreeImage_Save(FIF_PNG, img, scene.output_filename.c_str(), 0);
	
	// End Program Display Text
	std::cout << "\n********************" << "\n";
	std::cout << "Ray Tracing Complete" << "\n";
	std::cout << "\n********************" << "\n";
	FreeImage_DeInitialise();

	
	system("Pause");
}