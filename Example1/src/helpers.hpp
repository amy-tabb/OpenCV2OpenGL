/*
 * helpers.hpp
 *
 *  Created on: May 25, 2019
 *      Author: atabb
 */

#ifndef HELPERS_HPP_
#define HELPERS_HPP_

#include "includes.hpp"
#include <GLFW/glfw3.h>


string FindValueOfFieldInFile(string filename, string fieldTag, bool seperator);

MatrixXd FindValueOfFieldInFile(string filename, string fieldTag, int rows, int cols, bool seperator);

void EnsureDirHasTrailingBackslash(string& write_directory);

bool CheckExistenceOfDirectory(string write_directory);

int create_camera(Matrix3d& internal, MatrixXd& external, int r, int g, int b, int rows, int cols,
		string ply_file, double scale);

void PrintGLMMat4(glm::mat4& model, ofstream& out);

void processInput(GLFWwindow *window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void camera(Matrix3d& Kinv, float max_u, float max_v, float mag, vector< Vector3d >& vertex_coordinates );

#endif
