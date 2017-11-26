#include "scene.h"

// TODO remove
// !!!!!!!!!!!!!

void test_line(std::string section_string)
{
	std::cout << "made it to: " << section_string << "\n";
}
// !!!!!!!!!!!!!!!!!!!!



void scene::readfile(const char * filename)
{

	std::string str, cmd;
	std::ifstream in;
	in.open(filename);
	std::cout << "Parsing: " <<filename << "\n"; // TODO Don't need

	if (in.is_open())
	{
		getline(in, str);
		while (in)
		{
			if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#'))
			{
				// Ruled out comment and blank lines 
				std::stringstream s(str);
				s >> cmd;
				int i;
				float values[10]; // Position and color for light, colors for others
								// Up to 10 params for cameras. 
				bool validinput; // Validity of input 

				// parse camera data
				if (cmd == "camera")
				{
					validinput = readvals(s, 10, values); // 10 values eye cen up fov
					if (validinput)
					{
						std::string word = "camera input";
						test_line(word);
						// TODO Camera input for readfile
						// YOUR CODE FOR HW 2 HERE
						// Use all of values[0...9]
						// You may need to use the upvector fn in Transform.cpp
						// to set up correctly. 
						// Set eyeinit upinit center fovy in variables.h 
						LookFrom[0] = values[0];
						LookFrom[1] = values[1];
						LookFrom[2] = values[2];

						LookAt[0] = values[3];
						LookAt[1] = values[4];
						LookAt[2] = values[5];

						UpVec[0] = values[6];
						UpVec[1] = values[7];
						UpVec[2] = values[8];

						UpVec = camera::upvector(UpVec, LookFrom);

						fovy = values[9];
	//					printvec3(LookFrom);
						

					}
				}
			}
		}
	}
	else
	{
		std::cerr << "Unable to Open Input Data File " << filename << "\n";
		throw 2;
	}

}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool scene::readvals(std::stringstream &s, const int numvals, float* values)
{
	for (int i = 0; i < numvals; i++)
	{
		s >> values[i];
		if (s.fail())
		{
			std::cout << "Failed reading value " << i << " will skip\n";
			return false;
		}
	}
	return true;
}

scene::scene()
{
}

scene::~scene()
{
}