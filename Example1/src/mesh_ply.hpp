#ifndef MESH_PLY_H
#define MESH_PLY_H

#include <glad/glad.h> // holds all OpenGL type declarations
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ReadPly.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "shader.h"
using namespace std;

// This is largely adapted from the LearnOpenGL tutorial

class Mesh_Ply {
public:
	/*  Mesh Data  */
	vector<ply_vertex> vertices;
	vector<unsigned int> indices;
	// vector<Texture> textures;
	unsigned int VAO;

	Mesh_Ply(){
		VAO = 0; EBO = 0; VBO = 0;
	}

	void Initialize(vector<ply_vertex>& vertices, vector<unsigned int>& indices){
		this->vertices = vertices;
		this->indices = indices;

		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();
	}
	/*  Functions  */
	// constructor
	Mesh_Ply(vector<ply_vertex>& vertices, vector<unsigned int>& indices)
	{
		this->vertices = vertices;
		this->indices = indices;

		cout << "Before set up mesh .... " << endl;
		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();
		cout << "After set up mesh. " << endl;
	}

	// render the mesh
	void Draw(Shader shader)
	{
		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//
		//        // always good practice to set everything back to defaults once configured.
		//        glActiveTexture(GL_TEXTURE0);
	}

private:
	/*  Render data  */
	unsigned int VBO, EBO;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ply_vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ply_vertex), (void*)0);

		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ply_vertex), (void*)(3 * sizeof(float)));

		// color attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ply_vertex), (void*)(6 * sizeof(float)));

		glBindVertexArray(0);
	}
};
#endif
