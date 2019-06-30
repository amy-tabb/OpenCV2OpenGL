/*
 * ReadPly.hpp
 *
 *  Created on: May 25, 2019
 *      Author: atabb
 */

#ifndef READPLY_HPP_
#define READPLY_HPP_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <inttypes.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

struct ply_vertex {
	 glm::vec3 Position;
	 glm::vec3 Normal;
	glm::vec4 color;
};



class ply{

public:
	vector<ply_vertex> vertices;
	vector<unsigned int> face_indices;

	// assume all are triangles;
	uint64_t number_vertices;
	uint64_t number_faces;

	ply();

	~ply();

	void Read(string filename);

	void PrintBasics();
};


#endif /* READPLY_HPP_ */
