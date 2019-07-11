/*
 * OpenCV2OpenGL1.hpp
 *
 *  Created on: Jun 29, 2019
 *      Author: atabb
 */

#ifndef OPENCV2OPENGL1_HPP_
#define OPENCV2OPENGL1_HPP_
#pragma once

#include "includes.hpp"

int LoadVersion0(string input, string output, string writefile);

int LoadVersion1(string input, string output);

void ComputeOpenGL_FromCVParameters(Matrix3d& K, MatrixXd& Rt, Vector3d& C, glm::mat4& opengl_intrinsics,
		glm::mat4& opengl_extrinsics, glm::vec3& camera_center, double near, double far, double cols, double rows,
		ofstream& out);

#endif /* OPENCV2OPENGL1_HPP_ */
