//============================================================================
// Name        : OpenCV2OpenGL1.cpp
// Author      : Amy Tabb
// Version     :
// Copyright   : MIT
// Description :
//============================================================================

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include "OpenCV2OpenGL1.hpp"
#include "ReadPly.hpp"
#include "helpers.hpp"
#include "mesh_ply.hpp"


int main(int argc, char * argv[]) {
	int print_help = false;
	int verbose = false;
	string input_file;
	string output_directory;
	int version_number = 0;
	int opt_argument;
	string writename = "image.png";

	// input is a file.

	while (1)
	{
		static struct option long_options[] =
		{
				/* These options set a flag. */
				{"help",   no_argument,       &print_help, 1},
				/* These options don’t set a flag.
	             We distinguish them by their indices. */
				{"input",   required_argument, 0, 'a'},
				{"output",  required_argument, 0, 'b'},
				{"version",  required_argument, 0, 'c'},
				{"write-name", required_argument, 0, 'd'}
		};

		if (print_help == 1){
			cout << "Printing help for project OpenCV2OpenGL1. "<< endl;

			cout << "To select help, use the flag" << endl;
			cout << "--help " << endl;

			cout << "otherwise, use these flags with the following arguments:" << endl;
			cout << std::left << setw(42) << "--input=[STRING] "<< "text file with calibration information, Mandatory." << endl;
			cout << std::left << setw(42) << "--output=[STRING] " << "directory for writing output, Mandatory." << endl;
			cout << std::left << setw(42) << "--version=[INT] " << "(default is 0). number of version selected, more details in the README" << endl;
			cout << std::left << setw(42) << "--write-name=[STRING] " << "(default is image.png). filename for the resulting image file" << endl;
			exit(1);
		}
		/* getopt_long stores the option index here. */
		int option_index = 0;

		opt_argument = getopt_long (argc, argv, "abcd",
				long_options, &option_index);

		/* Detect the end of the options. */
		if (opt_argument == -1)
			break;

		switch (opt_argument)
		{
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
			printf ("option %s", long_options[option_index].name);
			if (optarg)
				printf (" with arg %s", optarg);
			printf ("\n");
			break;

		case 'a':
			puts ("option -a\n");
			input_file = optarg;
			cout << "Set " << input_file << endl;
			break;

		case 'b':
			puts ("option -b\n");
			output_directory = optarg;
			cout << "Set " << output_directory << endl;
			break;

		case 'c':
			printf ("option -c with value `%s'\n", optarg);

			version_number = FromString<int>(optarg);
			cout << "Version Number " << version_number << endl;

			break;
		case 'd':
			printf ("option -c with value `%s'\n", optarg);

			writename = optarg;
			cout << "Version Number " << version_number << endl;

			break;
		case '?':
			/* getopt_long already printed an error message. */
			break;

		default:
			abort ();
		}
	}

	/* Print any remaining command line arguments (not options). */
	if (optind < argc)
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		putchar ('\n');
	}

	///////////  Some basic input checking ....////////////
	// need to be able to tell if this is a directory or a file .. check if a dir.  If a dir, then fail.


	ifstream in;
	in.open(input_file.c_str());
	if (!in.good()){
		cout << "File is bad, quitting" << endl;
		exit(1);
	}

	EnsureDirHasTrailingBackslash(output_directory);


	if (!CheckExistenceOfDirectory(output_directory)){
		exit(1);
	}

	// TODO Now, call the appropriate function.
	switch (version_number){
	case 0: {
		LoadVersion0(input_file, output_directory, writename);
	} break;
	case 1: {
		LoadVersion1(input_file, output_directory);
	} break;

	default: {
		cout << "This case does not exist " << version_number << endl;
	}
	}


	exit (0);

}

int LoadVersion0(string input, string output, string writefile){

	string califile = input;
	string logfile = output + "logfile.txt";
	string meshfile = "";

	ofstream out;
	out.open(logfile.c_str());

	string fieldString;
	string fieldTag;
	string return_string;

	int image_rows = 1280;
	int image_cols = 960;
	Matrix3d K;
	MatrixXd RTworld2cam(3, 4);
	Matrix4d ModelTrans;
	ModelTrans.setIdentity();
	int near = 1;
	int far = 2000;
	int write_camera = false;
	double camera_scale = 40;
	Vector3d camera_color;  camera_color.setZero();
	camera_color(1) = 255;
	Vector3d EigLightPosition;  EigLightPosition.setZero();
	EigLightPosition(1) = 10;
	EigLightPosition(2) = 1000;

	///light
	glm::vec3 diffuseColor = glm::vec3(1.0f);
	glm::vec3 ambientColor = glm::vec3(0.5f);
	glm::vec3 specularColorLight = glm::vec3(1.0f);
	glm::vec3 materalSpecularColorLight = glm::vec3(0.3f);
	float shininess = 12.0;

	bool trans_present = false;


	glm::mat4 opengl_intrinsics;
	glm::mat4 opengl_extrinsics;
	glm::vec3 camera_center;

	glm::vec3 light_position;
	Vector3d C;
	vector<Mesh_Ply> MeshObjs;
	ply PlyModel;

	////////// READ IN /////////////////////////////////

	K.setIdentity();
	K(0, 0) = 500;  K(1, 1) = 500;
	K(0, 2) = image_cols/2;  K(1, 2) = image_rows/2;

	RTworld2cam.setZero();
	RTworld2cam(0,0) = 1;
	RTworld2cam(1,1) = 1;
	RTworld2cam(2,2) = 1;

	// This 'FindValueOfFieldInFile function is meant for small files.  Write something better if you
	// use larger files.  Defaults are used if a value is not specified.

	return_string = FindValueOfFieldInFile(califile, "rows",false);
	if (return_string.size() > 0){
		image_rows = FromString<int>(return_string);
	}


	return_string = FindValueOfFieldInFile(califile, "cols",false);
	if (return_string.size() > 0){
		image_cols = FromString<int>(return_string);
	}

	return_string = FindValueOfFieldInFile(califile, "near",false);
	if (return_string.size() > 0){
		near = FromString<int>(return_string);
	}

	return_string = FindValueOfFieldInFile(califile, "far",false);
	if (return_string.size() > 0){
		far = FromString<int>(return_string);
	}

	return_string = FindValueOfFieldInFile(califile, "write-camera", false);
	if (return_string.size() > 0){
		write_camera = FromString<int>(return_string);
	}

	if (write_camera == true){
		return_string = FindValueOfFieldInFile(califile, "camera-scale", false);
		if (return_string.size() > 0){
			camera_scale = FromString<double>(return_string);
		}

		MatrixXd testColor =  FindValueOfFieldInFile(califile, "camera-color", 1, 3, false);
		if (testColor.rows() == 1 && testColor.cols() == 3){
			for (int i = 0; i < 3; i++){
				camera_color(i) = testColor(0,i);
			}

		}


	}

	MatrixXd testK =  FindValueOfFieldInFile(califile, "K", 3, 3, false);
	if (testK.rows() == 3){
		K = testK;
	}

	MatrixXd testRT =  FindValueOfFieldInFile(califile, "RTworld2cam", 3, 4, false);
	if (testRT.rows() == 3 && testRT.cols() == 4){
		RTworld2cam = testRT;
	}


	MatrixXd testM =  FindValueOfFieldInFile(califile, "Model", 4, 4, false);
	if (testM.rows() == 4 && testM.cols() == 4){
		ModelTrans = testM;
		trans_present = true;
	}

	MatrixXd testLight =  FindValueOfFieldInFile(califile, "light-position", 1, 3, false);
	if (testLight.rows() == 1 && testLight.cols() == 3){
		for (int i = 0; i < 3; i++){
			EigLightPosition(i) = testLight(0,i);
		}

	}

	return_string =  FindValueOfFieldInFile(califile, "shininess", false);
	if (return_string.size() > 0){
		shininess = FromString<float>(return_string);
	}


	return_string =  FindValueOfFieldInFile(califile, "file", false);

	if (return_string.size() > 0){


		PlyModel.Read(return_string);
		PlyModel.PrintBasics();
	}	else {
		cout << "Error, no file 3D model file!  quitting." << endl;
	}

	cout << "Loaded!" << endl;

	///////////////////   WRITE TO THE LOG/////////////////
	out << "Log, version 2" << endl;

	out << "rows " << image_rows << endl;
	out << "cols " << image_cols << endl;
	out << "K-CV" << endl;
	out << K << endl;
	out << "RTworld2cam" << endl;
	out << RTworld2cam << endl;
	out << "Model Transformation" << endl;
	out << ModelTrans << endl;
	out << "near " << near << endl;
	out << "far " << far << endl;
	out << "Light position " << endl;
	out << EigLightPosition.transpose() << endl;

	string to_write = "";

	ComputeOpenGL_FromCVParameters(K, RTworld2cam, C, opengl_intrinsics,
			opengl_extrinsics, camera_center, near, far, image_cols, image_rows, out);

	for (int i = 0; i < 3; i++){
		light_position[i] = EigLightPosition(i);
	}

	out << "Computed OpenGl equivalent from my function: " << endl;
	out << "intrinsics: " << endl; PrintGLMMat4(opengl_intrinsics, out);
	out << "extrinsics: " << endl; PrintGLMMat4(opengl_extrinsics, out);
	out << "C " << endl << C << endl;

	// write the camera, before getting started with the OpenGL part.
	if (write_camera){
		string camera_file = output + "camera.ply";
		create_camera(K, RTworld2cam, camera_color(0), camera_color(1), camera_color(2), image_rows, image_cols,
				camera_file, camera_scale);

		if (trans_present){
			Matrix3d R;
			MatrixXd RT4(4, 4);  RT4.setZero();  RT4(3, 3) = 1;

			for (int r = 0; r < 3; r++){
				for (int c = 0; c < 4; c++){
					RT4(r, c) = RTworld2cam(r, c);
				}
			}

			RT4 = RT4*ModelTrans;

			for (int r = 0; r < 3; r++){
				for (int c = 0; c < 3; c++){
					R(r, c) = RT4(r, c);
				}
			}

			float det = R.determinant();

			if (det >= 0.999 && det <= 1.001){
				camera_file = output + "camera-rel-model.ply";

				create_camera(K, RT4, camera_color(0), camera_color(1), camera_color(2), image_rows, image_cols,
						camera_file, camera_scale);

			}	else {
				out << "cannot create camera-rel-model.ply file, because the model transformation is not Euclidean." << endl;
			}

		}
	}


	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(image_cols, image_rows, "Version 2", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);


	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	GLenum FORMAT = GL_RGBA;
	GLuint FORMAT_NBYTES = 4;

	// You may have to change the paths for the shaders, depending on how you run the program.
	Shader ourShader("../src/materials.vs", "../src/materials.fs");
	Shader lampShader("../src/lamp.vs", "../src/lamp.fs");

	MeshObjs.push_back(Mesh_Ply(PlyModel.vertices, PlyModel.face_indices));


	// Initialise a Mat to contain the image
	cv::Mat temp = cv::Mat::zeros(image_rows, image_cols, CV_8UC3); cv::Mat tempImage;

	//uchar image_storage[image_rows*image_cols][3];
	GLubyte *pixels = 0;
	pixels = new GLubyte[FORMAT_NBYTES * image_rows * image_cols];
	string filename;

	// render loop -- press escape to close the window
	// -----------
	int loop_counter = 0;
	while (!glfwWindowShouldClose(window))
	{

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(1.0f, 1.00f, 1.00f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();


		ourShader.setMat4("intrinsic", opengl_intrinsics);
		ourShader.setMat4("extrinsic", opengl_extrinsics);
		ourShader.setVec3("light.position", light_position);


		// need
		ourShader.setVec3("light.ambient", ambientColor);
		ourShader.setVec3("light.diffuse", diffuseColor);
		ourShader.setVec3("light.specular", specularColorLight);

		/// material
		ourShader.setVec3("material.ambient", ambientColor);
		ourShader.setVec3("material.diffuse", diffuseColor);
		ourShader.setVec3("material.specular", materalSpecularColorLight);
		ourShader.setFloat("material.shininess", shininess);


		// render the loaded model
		glm::mat4 model;
		model = glm::mat4(1.0);

		// load the Eigen version ...
		for (int r = 0; r < 3; r++){
			for (int c = 0; c < 4; c++){
				model[c][r] = ModelTrans(r, c);
			}
		}


		if (loop_counter == 0){
			cout << "model_trans" << endl;
			PrintGLMMat4(model, out);
		}


		ourShader.setMat4("model_trans", model);
		MeshObjs[0].Draw(ourShader);

		// is this really used???
		lampShader.use();
		lampShader.setMat4("projection", opengl_intrinsics);
		lampShader.setMat4("view", opengl_extrinsics);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.setMat4("model", model);


		// Read Image from buffer -- upside down.
		if (loop_counter == 0){
			glReadPixels(0, 0, image_cols, image_rows, FORMAT, GL_UNSIGNED_BYTE, pixels);
			Vec3b intensity;
			int index;
			for (int r = 0; r < image_rows; r++){
				for (int c = 0; c < image_cols; c++){
					index = FORMAT_NBYTES * ((image_rows - r - 1) * image_cols + c);

					for (int j = 0; j < 3; j++){
						intensity[j] = pixels[index + (2 - j)];
					}
					temp.at<Vec3b>(r, c) = intensity;
				}
			}


			string number_file = output + writefile;

			cv::imwrite(number_file.c_str(), temp);
			cout << "wrote file!" << endl;
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		loop_counter++;

	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


int LoadVersion1(string input, string output){

	string califile = input;
	string logfile = output + "logfile.txt";
	string meshfile = "";

	ofstream out;
	out.open(logfile.c_str());

	string fieldString;
	string fieldTag;
	string return_string;

	int image_rows = 1280;
	int image_cols = 960;
	Matrix3d K;
	MatrixXd RTworld2cam(3, 4);
	Matrix4d ModelTrans;
	ModelTrans.setIdentity();
	int near = 1;
	int far = 2000;
	int write_camera = false;
	double camera_scale = 40;
	float total_degrees = 180;
	float degrees_per_step = 5;
	int first_write_file_number = 0;
	float current_angle_rad;
	Vector3d rotation_vector; rotation_vector.setZero();
	rotation_vector(2) = 1;
	Vector3d camera_color;  camera_color.setZero();
	camera_color(1) = 255;
	Vector3d EigLightPosition;  EigLightPosition.setZero();
	EigLightPosition(1) = 10;
	EigLightPosition(2) = 1000;

	///light
	glm::vec3 diffuseColor = glm::vec3(1.0f);
	glm::vec3 ambientColor = glm::vec3(0.5f);
	glm::vec3 specularColorLight = glm::vec3(1.0f);
	glm::vec3 materalSpecularColorLight = glm::vec3(0.3f);
	float shininess = 12.0;

	bool trans_present = false;


	glm::mat4 opengl_intrinsics;
	glm::mat4 opengl_extrinsics;
	glm::vec3 camera_center;

	glm::vec3 light_position;
	Vector3d C;
	vector<Mesh_Ply> MeshObjs;
	ply PlyModel;

	////////// READ IN /////////////////////////////////

	K.setIdentity();
	K(0, 0) = 500;  K(1, 1) = 500;
	K(0, 2) = image_cols/2;  K(1, 2) = image_rows/2;

	RTworld2cam.setZero();
	RTworld2cam(0,0) = 1;
	RTworld2cam(1,1) = 1;
	RTworld2cam(2,2) = 1;

	// This 'FindValueOfFieldInFile function is meant for small files.  Write something better if you
	// use larger files.  Defaults are used if a value is not specified.

	return_string = FindValueOfFieldInFile(califile, "rows",false);
	if (return_string.size() > 0){
		image_rows = FromString<int>(return_string);
	}


	return_string = FindValueOfFieldInFile(califile, "cols",false);
	if (return_string.size() > 0){
		image_cols = FromString<int>(return_string);
	}

	return_string = FindValueOfFieldInFile(califile, "near",false);
	if (return_string.size() > 0){
		near = FromString<int>(return_string);
	}

	return_string = FindValueOfFieldInFile(califile, "far",false);
	if (return_string.size() > 0){
		far = FromString<int>(return_string);
	}

	return_string = FindValueOfFieldInFile(califile, "total-degrees",false);
	if (return_string.size() > 0){
		total_degrees = FromString<float>(return_string);
	}

	return_string = FindValueOfFieldInFile(califile, "degrees-per-step",false);
	if (return_string.size() > 0){
		degrees_per_step = FromString<float>(return_string);
	}

	return_string = FindValueOfFieldInFile(califile, "write-camera", false);
	if (return_string.size() > 0){
		write_camera = FromString<int>(return_string);
	}

	return_string = FindValueOfFieldInFile(califile, "first-file-number", false);
	if (return_string.size() > 0){
		first_write_file_number = FromString<int>(return_string);
	}



	if (write_camera == true){
		return_string = FindValueOfFieldInFile(califile, "camera-scale", false);
		if (return_string.size() > 0){
			camera_scale = FromString<double>(return_string);
		}

		MatrixXd testColor =  FindValueOfFieldInFile(califile, "camera-color", 1, 3, false);
		if (testColor.rows() == 1 && testColor.cols() == 3){
			for (int i = 0; i < 3; i++){
				camera_color(i) = testColor(0,i);
			}
		}
	}

	MatrixXd testK =  FindValueOfFieldInFile(califile, "K", 3, 3, false);
	if (testK.rows() == 3){
		K = testK;
	}

	MatrixXd testRT =  FindValueOfFieldInFile(califile, "RTworld2cam", 3, 4, false);
	if (testRT.rows() == 3 && testRT.cols() == 4){
		RTworld2cam = testRT;
	}


	MatrixXd testM =  FindValueOfFieldInFile(califile, "Model", 4, 4, false);
	if (testM.rows() == 4 && testM.cols() == 4){
		ModelTrans = testM;
		trans_present = true;
	}

	MatrixXd testLight =  FindValueOfFieldInFile(califile, "light-position", 1, 3, false);
	if (testLight.rows() == 1 && testLight.cols() == 3){
		for (int i = 0; i < 3; i++){
			EigLightPosition(i) = testLight(0,i);
		}
	}

	MatrixXd testRotVec =  FindValueOfFieldInFile(califile, "rotation-vector", 1, 3, false);
	if (testRotVec.rows() == 1 && testRotVec.cols() == 3){
		for (int i = 0; i < 3; i++){
			rotation_vector(i) = testRotVec(0,i);
		}
	}

	return_string =  FindValueOfFieldInFile(califile, "shininess", false);
	if (return_string.size() > 0){
		shininess = FromString<float>(return_string);
	}


	return_string =  FindValueOfFieldInFile(califile, "file", false);

	if (return_string.size() > 0){


		PlyModel.Read(return_string);
		PlyModel.PrintBasics();
	}	else {
		cout << "Error, no file 3D model file!  quitting." << endl;
	}

	cout << "Loaded!" << endl;

	///////////////////   WRITE TO THE LOG/////////////////
	out << "Log, version 2" << endl;

	out << "rows " << image_rows << endl;
	out << "cols " << image_cols << endl;
	out << "K-CV" << endl;
	out << K << endl;
	out << "RTworld2cam" << endl;
	out << RTworld2cam << endl;
	out << "Model Transformation" << endl;
	out << ModelTrans << endl;
	out << "near " << near << endl;
	out << "far " << far << endl;
	out << "Light position " << endl;
	out << EigLightPosition.transpose() << endl;

	int number_steps = round(total_degrees/degrees_per_step);

	string to_write = "";

	ComputeOpenGL_FromCVParameters(K, RTworld2cam, C, opengl_intrinsics,
			opengl_extrinsics, camera_center, near, far, image_cols, image_rows, out);

	for (int i = 0; i < 3; i++){
		light_position[i] = EigLightPosition(i);
	}

	out << "Computed OpenGl equivalent from my function: " << endl;
	out << "intrinsics: " << endl; PrintGLMMat4(opengl_intrinsics, out);
	out << "extrinsics: " << endl; PrintGLMMat4(opengl_extrinsics, out);
	out << "C " << endl << C << endl;

	// write the camera, before getting started with the OpenGL part.
	if (write_camera){
		string camera_file = output + "camera.ply";
		create_camera(K, RTworld2cam, camera_color(0), camera_color(1), camera_color(2), image_rows, image_cols,
				camera_file, camera_scale);

		if (trans_present){
			Matrix3d R;
			MatrixXd RT4(4, 4);  RT4.setZero();  RT4(3, 3) = 1;

			for (int r = 0; r < 3; r++){
				for (int c = 0; c < 4; c++){
					RT4(r, c) = RTworld2cam(r, c);
				}
			}

			RT4 = RT4*ModelTrans;

			for (int r = 0; r < 3; r++){
				for (int c = 0; c < 3; c++){
					R(r, c) = RT4(r, c);
				}
			}

			float det = R.determinant();

			if (det >= 0.999 && det <= 1.001){
				camera_file = output + "camera-rel-model.ply";

				create_camera(K, RT4, camera_color(0), camera_color(1), camera_color(2), image_rows, image_cols,
						camera_file, camera_scale);

			}	else {
				out << "cannot create camera-rel-model.ply file, because the model transformation is not Euclidean." << endl;
			}

		}
	}


	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(image_cols, image_rows, "Version 2", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);


	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	GLenum FORMAT = GL_RGBA;
	GLuint FORMAT_NBYTES = 4;

	// You may have to change the paths for the shaders, depending on how you run the program.
	Shader ourShader("../src/materials.vs", "../src/materials.fs");
	Shader lampShader("../src/lamp.vs", "../src/lamp.fs");

	MeshObjs.push_back(Mesh_Ply(PlyModel.vertices, PlyModel.face_indices));


	// Initialise a Mat to contain the image
	cv::Mat temp = cv::Mat::zeros(image_rows, image_cols, CV_8UC3); cv::Mat tempImage;

	GLubyte *pixels = 0;
	pixels = new GLubyte[FORMAT_NBYTES * image_rows * image_cols];
	string filename;

	// render loop -- press escape to close the window
	// -----------
	int loop_counter = 0;

	//while (!glfwWindowShouldClose(window))
	for (int step_counter = 0; step_counter < number_steps; step_counter++)
	{

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(1.0f, 1.00f, 1.00f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();


		ourShader.setMat4("intrinsic", opengl_intrinsics);
		ourShader.setMat4("extrinsic", opengl_extrinsics);
		ourShader.setVec3("light.position", light_position);


		// need
		ourShader.setVec3("light.ambient", ambientColor);
		ourShader.setVec3("light.diffuse", diffuseColor);
		ourShader.setVec3("light.specular", specularColorLight);

		/// material
		ourShader.setVec3("material.ambient", ambientColor);
		ourShader.setVec3("material.diffuse", diffuseColor);
		ourShader.setVec3("material.specular", materalSpecularColorLight);
		ourShader.setFloat("material.shininess", shininess);


		// render the loaded model
		glm::mat4 model;
		model = glm::mat4(1.0);

		glm::mat4 modelR;
		modelR = glm::mat4(1.0);

		// load the Eigen version ...
		for (int r = 0; r < 3; r++){
			for (int c = 0; c < 4; c++){
				model[c][r] = ModelTrans(r, c);
			}
		}

		current_angle_rad = float(step_counter)*degrees_per_step*0.0174533;

		modelR = glm::rotate(modelR, current_angle_rad, glm::vec3(rotation_vector(0), rotation_vector(1), rotation_vector(2)));

		model = modelR*model;



		{
			cout << "model rotated " << step_counter <<  endl;
			PrintGLMMat4(model, out);
		}


		ourShader.setMat4("model_trans", model);
		MeshObjs[0].Draw(ourShader);

		// is this really used???
		lampShader.use();
		lampShader.setMat4("projection", opengl_intrinsics);
		lampShader.setMat4("view", opengl_extrinsics);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.setMat4("model", model);


		// Read Image from buffer -- upside down.
		{
			glReadPixels(0, 0, image_cols, image_rows, FORMAT, GL_UNSIGNED_BYTE, pixels);
			Vec3b intensity;
			int index;
			for (int r = 0; r < image_rows; r++){
				for (int c = 0; c < image_cols; c++){
					index = FORMAT_NBYTES * ((image_rows - r - 1) * image_cols + c);

					for (int j = 0; j < 3; j++){
						intensity[j] = pixels[index + (2 - j)];
					}
					temp.at<Vec3b>(r, c) = intensity;
				}
			}


			string number_file = output + ToString<int>(first_write_file_number + step_counter) + ".png";

			cv::imwrite(number_file.c_str(), temp);
			cout << "wrote file!" << endl;
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		loop_counter++;

	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


void ComputeOpenGL_FromCVParameters(Matrix3d& K, MatrixXd& Rt, Vector3d& C, glm::mat4& opengl_intrinsics,
		glm::mat4& opengl_extrinsics, glm::vec3& camera_center, double near, double far, double cols, double rows,
		ofstream& out){

	/// Compute camera center position.
	Vector3d t;
	for (int r = 0; r < 3; r++){
		t(r) = Rt(r, 3);
	}

	Matrix3d R;
	for (int r = 0; r < 3; r++){
		for (int c = 0; c < 3; c++){
			R(r, c) = Rt(r, c);
		}
	}

	C = -R.transpose()*t;

	for (int r = 0; r < 3; r++){
		camera_center[r] = C(r);
	}

	glm::mat4 perspective = glm::mat4(1.0);
	glm::mat4 ndc = glm::mat4(1.0);

	double A = -(near + far);
	double B = near*far;
	/// compute the OpenGL equivalent.
	opengl_intrinsics = glm::mat4(1.0);
	// different indexing.
	perspective[0][0] = -K(0,0);
	perspective[1][1] = -K(1,1);
	perspective[1][0] = -K(0,1);
	perspective[2][0] = cols - K(0,2);
	perspective[2][1] = rows - K(1,2);
	perspective[2][2] = A;
	perspective[3][2] = B;
	perspective[3][3] = 0;
	perspective[2][3] = 1;


	ndc[0][0] = -2.0/cols;
	ndc[1][1] = 2.0/rows;
	ndc[3][0] = 1;
	ndc[3][1] = -1;
	ndc[2][2] = -2.0/(far - near);
	ndc[3][2] = -(far+near)/(far-near);


	out << "K-GL " << endl;
	PrintGLMMat4(perspective, out);
	out << "NDC " << endl;
	PrintGLMMat4(ndc, out);


	MatrixXd conversionT(3, 4); conversionT.setZero();

	conversionT(0, 0) = cols/2.0;
	conversionT(0, 3) = cols/2.0;
	conversionT(1, 1) = rows/2.0;
	conversionT(1, 3) = rows/2.0;
	conversionT(2, 3) = 1;

	out << "Conversion Matrix -- convert from image coords in OpenGL to OpenCV" << endl;
	out << conversionT << endl;

	opengl_intrinsics = ndc*perspective;

	/// then copy over the extrinsic matrix, taking into account the reversal of the indices with glm.
	opengl_extrinsics = glm::mat4(1.0);
	for (int r = 0; r < 3; r++){
		for (int c = 0; c < 4; c++){
			opengl_extrinsics[c][r] = Rt(r, c);
		}
	}

}
