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

				// lights


				// material properties
				else if (cmd == "diffuse")
				{
					validinput = readvals(s, 3, values);
					diffuse = glm::vec3(values[0], values[1], values[2]);
				}
				else if (cmd == "specular")
				{
					validinput = readvals(s, 3, values);
					specular = glm::vec3(values[0], values[1], values[2]);
				}
				else if (cmd == "emission")
				{
					validinput = readvals(s, 3, values);
					emission = glm::vec3(values[0], values[1], values[2]);
				}
				else if (cmd == "shininess")
				{
					validinput = readvals(s, 1, values);
					shininess = values[0];
				}
				else if (cmd == "ambient")
				{
					validinput = readvals(s, 3, values);
					ambient = glm::vec3(values[0], values[1], values[2]);
				}

				//geometry
				else if (cmd == "maxverts")
				{
					validinput = readvals(s, 1, values);
					max_verts = values[0];
				}
				else if (cmd == "vertex")
				{
					validinput = readvals(s, 3, values);
					v_vertices.push_back(glm::vec3(values[0], values[1], values[2]));
				}
				else if (cmd == "tri")
				{
					validinput = readvals(s, 3, values);
					v_primitives.push_back(new triangle(v_vertices[values[0]],
														v_vertices[values[1]],
														v_vertices[values[2]]));
					// assign material properties
					v_primitives.back()->prim_diffuse = diffuse;
					v_primitives.back()->prim_specular = specular;
					v_primitives.back()->prim_emission = emission;
					v_primitives.back()->prim_shininess = shininess;
					v_primitives.back()->prim_ambient = ambient;
				}
				else if (cmd == "sphere")
				{
					validinput = readvals(s, 4, values);
					v_primitives.push_back(new sphere(glm::vec3(values[0], values[1], values[2]), // center
 													values[3]) // radius
													);
					// assign material properties
					v_primitives.back()->prim_diffuse = diffuse;
					v_primitives.back()->prim_specular = specular;
					v_primitives.back()->prim_emission = emission;
					v_primitives.back()->prim_shininess = shininess;
					v_primitives.back()->prim_ambient = ambient;

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