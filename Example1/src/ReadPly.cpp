/*
 * ReadPly.cpp
 *
 *  Created on: May 25, 2019
 *      Author: atabb
 */

#include "ReadPly.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <stdio.h>

using namespace std;



ply::ply(){
	number_faces = 0;
	number_vertices = 0;
}

void ply::Read(string filename){

	std::ifstream in(filename.c_str());

	if (!in){
		cout << "filename bad " << filename << endl;
		exit(1);
	}	else {
		// read....
		// first, establish that this is a ply file.
		string read_string;
		string s0, s1, s2;

		in >> read_string;

		if (read_string.compare(string("ply")) != 0){
			cout << "This file is not a ply file" << endl;
			exit(1);
		}

		while (read_string.compare(string("element")) != 0){
			in >> read_string;
		}

		in >> read_string;

		if (read_string.compare(string("vertex")) != 0){
			cout << "Order not correct -- expecting vertex" << endl;
			exit(1);
		}

		in >> number_vertices;

		// verify that the next properties are x, y, z, nx, ny, nz, colorr, colorg, colorb, coloralpha, then faces.
		in >> s0 >> s1 >> s2;
		if (s0.compare("property") != 0 || s2.compare("x") != 0){
			cout << "Order not correct -- expecting property -type- x" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << s2 << endl;
			exit(1);
		}

		in >> s0 >> s1 >> s2;
		if (s0.compare("property") != 0 || s2.compare("y") != 0){
			cout << "Order not correct -- expecting property -type- y" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << s2 << endl;
			exit(1);
		}

		in >> s0 >> s1 >> s2;
		if (s0.compare("property") != 0 || s2.compare("z") != 0){
			cout << "Order not correct -- expecting property -type- z" << endl;
			exit(1);
		}

		in >> s0 >> s1 >> s2;
		if (s0.compare("property") != 0 || s2.compare("nx") != 0){
			cout << "Order not correct -- expecting property -type- x" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << s2 << endl;
			exit(1);
		}

		in >> s0 >> s1 >> s2;
		if (s0.compare("property") != 0 || s2.compare("ny") != 0){
			cout << "Order not correct -- expecting property -type- ny" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << s2 << endl;
			exit(1);
		}

		in >> s0 >> s1 >> s2;
		if (s0.compare("property") != 0 || s2.compare("nz") != 0){
			cout << "Order not correct -- expecting property -type- nz" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << s2 << endl;
			exit(1);
		}


		in >> s0 >> s1 >> s2;
		if (s0.compare("property") != 0 || s1.compare("uchar") != 0||  s2.compare("red") != 0){
			cout << "Order not correct -- expecting property uchar red" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << s2 << endl;
			exit(1);
		}

		in >> s0 >> s1 >> s2;
		if (s0.compare("property") != 0 || s1.compare("uchar") != 0||  s2.compare("green") != 0){
			cout << "Order not correct -- expecting property uchar green" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << s2 << endl;
			exit(1);
		}

		in >> s0 >> s1 >> s2;
		if (s0.compare("property") != 0 || s1.compare("uchar") != 0||  s2.compare("blue") != 0){
			cout << "Order not correct -- expecting property uchar blue" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << s2 << endl;
			exit(1);
		}

		in >> s0 >> s1 >> s2;
		if (s0.compare("property") != 0 || s1.compare("uchar") != 0||  s2.compare("alpha") != 0){
			cout << "Order not correct -- expecting property uchar alpha" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << s2 << endl;
			exit(1);
		}

		in >> s0 >> s1;
		if (s0.compare("element") != 0 || s1.compare("face") != 0){
			cout << "Order not correct -- expecting element face" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << endl;
			exit(1);
		}

		in >> number_faces;

		in >> s0 >> s1 >> s2;

		if (s0.compare("property") != 0 || s1.compare("list") != 0){
			cout << "Order not correct -- expecting property list --type--" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << s2 << endl;
			exit(1);
		}

		in >> s0 >> s1 >> s2;

		if (s0.compare("int") != 0 || s1.compare("vertex_indices") != 0 || s2.compare("end_header") != 0){
			cout << "Order not correct -- expecting int vertex_indices end_header" << endl;
			cout << "instead, " << s0 << " " << s1 << " " << s2 << endl;
			exit(1);
		}

		double dx, dy, dz;
		double c1, c2, c3, c4;
		uint64_t uindex;
		ply_vertex pv;

		for (unsigned int i = 0; i < number_vertices; i++){
			in >> dx; in >> dy; in >> dz;

			pv.Position = glm::vec3(dx, dy, dz);

			in >> dx; in >> dy; in >> dz;

			pv.Normal = glm::vec3(dx, dy, dz);

			in >> c1; in >> c2; in >> c3; in >> c4;

			pv.color = glm::vec4(c1/255.0, c2/255.0, c3/255.0, c4/255.0);
			vertices.push_back(pv);

		}

		for (unsigned int i = 0; i < number_faces; i++){
			in >> uindex;
			if (uindex != 3){
				cout << "Error!  Not triangles ..." << uindex << endl;  exit(1);
			}
			for (int j = 0; j < 3; j++){
				in >> uindex;  face_indices.push_back(uindex);
			}

		}
	}

}



void ply::PrintBasics(){
	cout << number_vertices << " vertices and " << number_faces << "  faces " << endl;
}

ply::~ply(){

}
