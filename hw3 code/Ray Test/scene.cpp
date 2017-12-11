#include "scene.h"

light::light()
{
}

light::~light()
{
}

light::light(glm::vec4 dir_pos_con, glm::vec4 color_con, bool attenutation_con)
{
	dir_pos = dir_pos_con;
	color = color_con;
	use_attenuation = attenutation_con;
}

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
		// TODO Model-View Matrix


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
				else if (cmd == "output")
				{
					s >> output_filename;
				}

				// lights
				else if (cmd == "directional")
				{
					validinput = readvals(s, 6, values);
					if (validinput)
					{
						light* temp_light = new light(glm::vec4(values[0], values[1], values[2], 0.f), // direction
													  glm::vec4(values[3], values[4], values[5], 1.f), // color
													  false);									  // false, directional lights do not use attenuation
						v_scene_lights.push_back(temp_light);
					}
				}
				else if (cmd == "point")
				{
					validinput = readvals(s, 6, values);
					if (validinput)
					{
						light* temp_light = new light(glm::vec4(values[0], values[1], values[2], 1.f), // direction
											    	  glm::vec4(values[3], values[4], values[5], 1.f), // color
												  	  true);									  // true, point lights do use attenuation
						v_scene_lights.push_back(temp_light);
					}
				}
				else if (cmd == "attenuation")
				{
					validinput = readvals(s, 3, values);
					if (validinput)
					{
						attenuation = glm::vec3(values[0], values[1], values[2]);
					}
				}

				// material properties
				else if (cmd == "diffuse")
				{
					validinput = readvals(s, 3, values);
					if (validinput) diffuse = glm::vec3(values[0], values[1], values[2]);
				}
				else if (cmd == "specular")
				{
					validinput = readvals(s, 3, values);
					if(validinput) specular = glm::vec3(values[0], values[1], values[2]);
				}
				else if (cmd == "emission")
				{
					validinput = readvals(s, 3, values);
					if(validinput) emission = glm::vec3(values[0], values[1], values[2]);
				}
				else if (cmd == "shininess")
				{
					validinput = readvals(s, 1, values);
					if(validinput) shininess = values[0];
				}
				else if (cmd == "ambient")
				{
					validinput = readvals(s, 3, values);
					if(validinput) ambient = glm::vec3(values[0], values[1], values[2]);
				}

				//geometry
				else if (cmd == "maxverts")
				{
					validinput = readvals(s, 1, values);
					if(validinput) max_verts = values[0];
				}
				else if (cmd == "vertex")
				{
					validinput = readvals(s, 3, values);
					if (validinput) v_vertices.push_back(glm::vec3(values[0], values[1], values[2]));
				}
				else if (cmd == "tri")
				{
					validinput = readvals(s, 3, values);
					if (validinput)
					{
						v_primitives.push_back(new triangle(v_vertices[values[0]],
															v_vertices[values[1]],
															v_vertices[values[2]]));
						// assign material properties
						v_primitives.back()->prim_diffuse = diffuse;
						v_primitives.back()->prim_specular = specular;
						v_primitives.back()->prim_emission = emission;
						v_primitives.back()->prim_shininess = shininess;
						v_primitives.back()->prim_ambient = ambient;
						v_primitives.back()->m_transform_stack = transform_stack.top();
						v_primitives.back()->m_transform_stack_inv = glm::inverse(transform_stack.top());
					}
				}
				else if (cmd == "sphere")
				{
					validinput = readvals(s, 4, values);
					if (validinput)
					{
						v_primitives.push_back(new sphere(glm::vec3(values[0], values[1], values[2]), // center
																	values[3]) // radius
																	);
						// assign material properties
						v_primitives.back()->prim_diffuse = diffuse;
						v_primitives.back()->prim_specular = specular;
						v_primitives.back()->prim_emission = emission;
						v_primitives.back()->prim_shininess = shininess;
						v_primitives.back()->prim_ambient = ambient;
						v_primitives.back()->m_transform_stack = transform_stack.top();
						v_primitives.back()->m_transform_stack_inv = glm::inverse(transform_stack.top());
					}

				}

				// transforms
				else if (cmd == "translate")
				{
					validinput = readvals(s, 3, values);
					if (validinput)
					{
						glm::mat4 translate_matrix = camera::translate(values[0], values[1], values[2]);
						rightmultiply(translate_matrix, transform_stack);
					}

				}
				else if (cmd == "scale")
				{
					validinput = readvals(s, 3, values);
					if (validinput)
					{
						glm::mat4 scale_matrix = camera::scale(values[0], values[1], values[2]);
						rightmultiply(scale_matrix, transform_stack);
					}
				}
				else if (cmd == "rotate")
				{
					validinput = readvals(s, 4, values);
					if (validinput)
					{
						glm::vec3 temp_vec = glm::vec3(values[0], values[1], values[2]);
						// TODO is this needed?		temp_vec = normalize(temp_vec);
						glm::mat4 rotation_mat = glm::mat4(camera::rotate(values[3], temp_vec));
						rightmultiply(rotation_mat, transform_stack);
					}
				}

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

void scene::rightmultiply(const glm::mat4 & M, std::stack<glm::mat4>& transfstack)
{
	glm::mat4 &T = transfstack.top();
	T = T * M;
}

void scene::matransform(std::stack<glm::mat4>& transfstack, float * values)
{
	glm::mat4 transform = transfstack.top();
	glm::vec4 valvec = glm::vec4(values[0], values[1], values[2], values[3]);
	glm::vec4 newval = transform * valvec;
	for (int i = 0; i < 4; i++) values[i] = newval[i];
}

scene::scene()
{
	attenuation = glm::vec3(0.f, 0.f, 1.f); // TODO check default attenuation (0.f, 0.f, 1.f) for point light?
}

scene::~scene()
{
}