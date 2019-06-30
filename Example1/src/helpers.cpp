/*
 * helpers.cpp
 *
 *  Created on: May 25, 2019
 *      Author: atabb
 */

#include "includes.hpp"
#include "helpers.hpp"

string FindValueOfFieldInFile(string filename, string fieldTag, bool seperator){

	/// reopen file each time, in case things get switched around.  Assume that these are very small files, not the most efficient.

	ifstream in(filename.c_str());

	if (!in.good()){
		cout << "Filename to find " << fieldTag << " is bad " << filename << " quitting !" << endl;
		exit(1);
	}

	string cmp_str;
	string read_str;



	int line_counter = 0;
	vector<string> tokens;
	string token;
	string return_str = "";
	bool found = false;


	while (in  && found == false){

		in >> token;


		if (token.compare(fieldTag) == 0){
			found = true;

			if (seperator == true && in){
				in >> token;
			}

			if (in){
				in >> return_str;
			}

		}


		line_counter++;
	}


	cout << "Found! " << found << " field " << fieldTag << " and result " << return_str << endl;
	in.close();

	return return_str;

}

MatrixXd FindValueOfFieldInFile(string filename, string fieldTag, int rows, int cols, bool seperator){

	/// reopen file each time, in case things get switched around.  Assume that these are very small files, not the most efficient.

	ifstream in(filename.c_str());

	if (!in.good()){
		cout << "Filename to find " << fieldTag << " is bad " << filename << " quitting !" << endl;
		exit(1);
	}

	string cmp_str;
	string read_str;


	cout << "rows, cols in find value " << rows << ", " << cols << endl;
	int line_counter = 0;
	vector<string> tokens;
	string token;
	string return_str = "";
	bool found = false;

	MatrixXd M(rows, cols);

	while (in  && found == false){

		in >> token;


		if (token.compare(fieldTag) == 0){
			found = true;

			if (seperator == true && in){
				in >> token;
			}

			if (in){
				for (int i = 0; i < rows; i++){
					for (int j = 0; j < cols; j++){
						in >> M(i, j);
					}
				}

			}

		}


		line_counter++;
	}


	cout << "Found! " << found << " field " << fieldTag << " and result " << return_str << endl;
	in.close();

	if (found){
		return M;
	}	else {
		MatrixXd M0;
		return M0;
	}
}

void EnsureDirHasTrailingBackslash(string& write_directory){
	int n_letters = write_directory.size();
	bool eval =  (write_directory[n_letters - 1] == '/');
	cout << "Last character compare " << write_directory << " " <<  eval << endl;
	if (eval == false){
		write_directory = write_directory + "/";
	}

}

bool CheckExistenceOfDirectory(string write_directory){

	bool exists= true;
	struct stat info;
	if( stat( write_directory.c_str(), &info ) != 0 ){
		cout << "Path to directory is wrong and/or cannot access " << write_directory << endl;
		exists = false;
	}

	return exists;

}


void camera(Matrix3d& Kinv, float max_u, float max_v, float mag, vector< Vector3d >& vertex_coordinates ) {

	Vector3d a;
	Vector3d b;
	Vector3d c;
	Vector3d d;

	//cout << "max u , max v, mag " << max_u << ", " << max_v << ", " << mag << endl;
	//cout << "Kinv " << endl << Kinv << endl;
	//
	//
	//	Vector4d C;
	//	C << 0, 0, 0, 1;

	Vector3d x;
	x << 0, 0, 1;

	a = mag*Kinv*x;

	//cout << "a, " << endl << a << endl;

	x << max_u, 0, 1;
	b = mag*Kinv*x;

	x << max_u, max_v, 1;
	c = mag*Kinv*x;

	x << 0, max_v, 1;
	d = mag*Kinv*x;

	vertex_coordinates.push_back(a);
	vertex_coordinates.push_back(d);
	vertex_coordinates.push_back(c);
	vertex_coordinates.push_back(b);



}

int create_camera(Matrix3d& internal, MatrixXd& external, int r, int g, int b, int rows, int cols,
		string ply_file, double scale){

	vector< Vector3d > vertex_coordinates;
	vector< Vector3d > vertex_normals;
	vector< vector<int> > face_indices;
	vector< vector<int> > edge_indices;
	vector<Vector3d> colors;
	Vector3d C;
	Vector3d t;
	Matrix3d R;
	// t = -RC, so C = -inv(R)*t
	for (int r0 = 0; r0 < 3; r0++){
		for (int c = 0; c < 3; c++){
			R(r0, c) = external(r0, c);
		}

		t(r0) = external(r0, 3);
	}


	C = -R.inverse()*t;

	Matrix3d Kinv = internal.inverse();


	Matrix3d Rinv = R.transpose();

	//R = Rtemp;
	cout << "camera VIS line 168 cols, rows  " << cols << ", " << rows << endl; //current_position[0] << ", " << current_position[1] << ", " << current_position[2]

	camera(Kinv, cols, rows, scale, vertex_coordinates);


	Vector3d tempV;


	for (int i = 0; i < 4; i++){
		tempV = vertex_coordinates[i];
		vertex_coordinates[i] = Rinv*(vertex_coordinates[i] - t);

	}

	Vector3d diff_vector;

	vertex_coordinates.push_back(C);


	Vector3d cp;
	int vertex_number = 0;

	// front face.
	vector<int> face;
	face.push_back(vertex_number);
	face.push_back(vertex_number + 3);
	face.push_back(vertex_number + 1);

	face_indices.push_back(face);


	face.clear();

	face.push_back(vertex_number + 2);
	face.push_back(vertex_number + 1);
	face.push_back(vertex_number + 3);

	face_indices.push_back(face);


	face.clear();
	// a side.

	face.push_back(vertex_number);
	face.push_back(vertex_number + 4);
	face.push_back(vertex_number + 3);


	face_indices.push_back(face);

	face.clear();

	face.push_back(vertex_number);
	face.push_back(vertex_number + 1);
	face.push_back(vertex_number + 4);


	face_indices.push_back(face);

	face.clear();

	face.push_back(vertex_number + 1);
	face.push_back(vertex_number + 2);
	face.push_back(vertex_number + 4);


	face_indices.push_back(face);

	face.clear();

	face.push_back(vertex_number + 2);
	face.push_back(vertex_number + 3);
	face.push_back(vertex_number + 4);


	face_indices.push_back(face);

	face.clear();


	vertex_number += 5;


	std::ofstream out;
	out.open(ply_file.c_str());


	out << "ply" << endl;
	out << "format ascii 1.0" << endl;
	out << "element vertex " << vertex_coordinates.size() << endl;
	out << "property float x" << endl;
	out << "property float y" << endl;
	out << "property float z" << endl;
	//out << "property float nx" << endl;
	//out << "property float ny" << endl;
	//out << "property float nz" << endl;
	out << "property uchar red" << endl;
	out << "property uchar green" << endl;
	out << "property uchar blue" << endl;
	out << "property uchar alpha" << endl;
	out << "element face " << face_indices.size() << endl;
	out << "property list uchar int vertex_indices"<< endl;

	out << "end_header" << endl;

	unsigned int zero = 0;
	for (int i = 0, nc = vertex_coordinates.size(); i < nc; i++){
		out << vertex_coordinates[i](0) << " " << vertex_coordinates[i](1) << " " << vertex_coordinates[i](2) << " ";

		if (i == 0){
			out << " 255 255 255 255" << endl;
		}	else {
			out << r << " " << g << " " << b << " 255 " << endl;
		}


	}

	for (int i = 0; i < int(face_indices.size()); i++){

		out << face_indices[i].size() << " ";


		for (int j = 0; j < int(face_indices[i].size()); j++){
			out << face_indices[i].at(j) << " ";
		}


		out << endl;

	}


	out.close();


	return 0;
}

