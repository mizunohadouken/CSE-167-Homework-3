#include <iostream>
#include "FreeImage.h"
#include <GLFW/glfw3.h>

int main(int argc, char *argv[])
{
	
	FreeImage_Initialise();
	std::cout << "Free Image_" << FreeImage_GetVersion() << "\n";
	std::cout << FreeImage_GetCopyrightMessage() << "\n\n";
	FreeImage_DeInitialise();
	system("Pause");
}