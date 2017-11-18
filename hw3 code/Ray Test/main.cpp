#include <iostream>
#include "FreeImage.h"
#include <GLFW/glfw3.h>

int main(int argc, char *argv[])
{
	
	FreeImage_Initialise();

	int Width = 500, Height = 500, BitsPerPixel = 24; // TODO change variables based on parser


	FIBITMAP *bitmap = FreeImage_Allocate(Width, Height, BitsPerPixel);
	RGBQUAD color;

	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			color.rgbRed = (double)i / Width * 255.0;
			color.rgbGreen = 0;
			color.rgbGreen = (double)j / Height * 255.0;
			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}

	FreeImage_Save(FIF_PNG, bitmap, "filename.png", 0);



	// Display Test
	std::cout << "Free Image_" << FreeImage_GetVersion() << "\n";
	std::cout << FreeImage_GetCopyrightMessage() << "\n\n";
	FreeImage_DeInitialise();
	system("Pause");
}