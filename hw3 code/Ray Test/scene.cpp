#include "scene.h"





void scene::readfile(const char * filename)
{

	std::string str, cmd;
	std::ifstream in;
	in.open(filename);
	std::cout << "Parsing: " <<filename << "\n"; // TODO Remove, Don't need

	if (in.is_open())
	{
		std::stack <glm::mat4> transform_stack;
		transform_stack.push(glm::mat4(1.0f)); // push identity matrix to top of stack


		getline(in, str);
		while (in)
		{
			if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#'))
			{
				// Ruled out comment and blank lines 
				std::stringstream s(str);
				s >> cmd;
				int i;
				float values[10]; // temp array for Position and color for light, colors for others
								// Up to 10 params for cameras. 
				bool validinput; // Tests validity of input 

				// parse camera data
				if (cmd == "camera")
				{
					validinput = readvals(s, 10, values); // 10 values eye cen up fov
					if (validinput)
					{
						// Parse data into scene class variables
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
					}
				}
				else if (cmd == "size")
				{
					validinput = readvals(s, 2, values);
					if (validinput)
					{
						width = values[0];
						height = values[1];
					}
				}
				else if (cmd == "sphere")
				{
					validinput = readvals(s, 4, values);

					v_primitives.push_back(new sphere(glm::vec3(values[0], values[1], values[2]), // center
 													values[3]) // radius
													);
				}

				// I include the basic push/pop code for matrix stacks
				else if (cmd == "pushTransform")
				{
					transform_stack.push(transform_stack.top());
				}
				else if (cmd == "popTransform")
				{
					if (transform_stack.size() <= 1)
					{
						std::cerr << "Stack has no elements.  Cannot Pop\n";
					}
					else
					{
						transform_stack.pop();
					}
				}
				else
				{
					std::cerr << "Unknown Command: " << cmd << " Skipping \n";
				}
			}
			getline(in, str);
		}
	}
	else
	{
		std::cerr << "Unable to Open Input Data File " << filename << "\n";
		throw 2;
	}

}

// Function to read the inputstring data and then temp store in values array
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