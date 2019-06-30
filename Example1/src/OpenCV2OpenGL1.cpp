//============================================================================
// Name        : OpenCV2OpenGL1.cpp
// Author      : Amy Tabb
// Version     :
// Copyright   : MIT
// Description :
//============================================================================

#include <GLFW/glfw3.h>
#include "OpenCV2OpenGL1.hpp"
#include "ReadPly.hpp"
#include "helpers.hpp"


int main(int argc, char * argv[]) {
	int print_help = false;
	int verbose = false;
	string input_file;
	string output_directory;
	int version_number = -1;
	int opt_argument;
	string writename = "image.png";

	// input is a file.

	while (1)
	{
		static struct option long_options[] =
		{
				/* These options set a flag. */
				{"help",   no_argument,       &print_help, 1},
				{"verbose",   no_argument,       &verbose, 1},
				/* These options don’t set a flag.
	             We distinguish them by their indices. */
				{"input",   required_argument, 0, 'a'},
				{"output",  required_argument, 0, 'b'},
				{"version",  required_argument, 0, 'c'},
				{"write_name", required_argument, 0, 'd'}
		};

		if (print_help == 1){
			cout << "Printing help for TODO"<< endl;

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

	if (print_help){
		cout << "Print help has been selected " << endl;

		//TODO write all of the commands





	}	else {





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
			LoadVersion0(input_file, output_directory, writename, verbose);
		} break;

		default: {
			cout << "This case does not exist " << version_number << endl;
		}
		}
	}






	exit (0);

}

int LoadVersion0(string input, string output, string writefile, int verbose){


}
