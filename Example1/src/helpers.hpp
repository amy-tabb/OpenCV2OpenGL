/*
 * helpers.hpp
 *
 *  Created on: May 25, 2019
 *      Author: atabb
 */

#ifndef HELPERS_HPP_
#define HELPERS_HPP_

#include "includes.hpp"


string FindValueOfFieldInFile(string filename, string fieldTag, bool seperator);

MatrixXd FindValueOfFieldInFile(string filename, string fieldTag, int rows, int cols, bool seperator);

void EnsureDirHasTrailingBackslash(string& write_directory);

bool CheckExistenceOfDirectory(string write_directory);

int create_camera(Matrix3d& internal, MatrixXd& external, int r, int g, int b, int rows, int cols,
		string ply_file, double scale);

#endif
