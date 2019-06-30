#ifndef MESH_PLY_H
#define MESH_PLY_H

//#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ReadPly.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "shader.hpp"
using namespace std;

class Mesh_Ply {
public:
	/*  Mesh Data  */
	vector<ply_vertex> vertices;
	vector<unsigned int> indices;
	// vector<Texture> textures;
	unsigned int VAO;

	Mesh_Ply();


	void Initialize(vector<ply_vertex>& vertices, vector<unsigned int>& indices);
	/*  Functions  */
	// constructor
	Mesh_Ply(vector<ply_vertex>& vertices, vector<unsigned int>& indices);


	// render the mesh
	void Draw(Shader shader);

private:
	/*  Render data  */
	unsigned int VBO, EBO;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh();
};


#endif
