#define _CRT_SECURE_NO_WARNINGS

#include "IOutils.h"
#include "myBFS.h"

using namespace std;

int main() {
	char* filename = new char[30];

	for(;;) {
		// input file
		cout << "Choose input BMP file name:\n 1)maze.bmp\n 2)maze2.bmp\n " << 
			"3)maze3.bmp\n 4)1024.bmp\n 5)cat.bmp\n 6)theseus.bmp\n x)Other\n";

		switch (_getch()) {
		case '1': 
			strcpy(filename, "maze1.bmp");   
			cout << "Recommended\n exit point: 1 30\n start point: 29 1\n"; 
			break;

		case '2': 
			strcpy(filename, "maze2.bmp");
			cout << "Recommended\n exit position: 56 0\n start: 13 20\n";
			break;

		case '3':
			strcpy(filename, "maze3.bmp");
			cout << "Recommended\n exit position: 73 93\n start: 9 9\n";
			break;

		case '4': 
			strcpy(filename, "1024.bmp");
			cout << "Recommended\n exit position: 1021 1023\n start: 0 2\n";
			break;

		case '5': 
			strcpy(filename, "cat.bmp");
			cout << "Recommended\n exit position: 230 600\n start: 20 70\n";
			break;

		case '6': 
			strcpy(filename, "theseus.bmp");
			cout << "Recommended\n exit position: 400 530\n start: 740 510\n";
			break;

		default: 
			cout << "Please Enter a file name: ";
			cin >> filename;
			break;
		}

		std::ifstream input(filename, std::ios::binary);

		// copies all data from binary file into imgdata
		std::vector<unsigned char> imgdata((
			std::istreambuf_iterator<char>(input)),
			(std::istreambuf_iterator<char>()));

		input.close();

		if (imgdata.size() == 0) {
			cout << "File not found\n" <<
					"Try again? y/n \n"; 
			if (_getch() != 'y')
				break;
		}

		// create output file
		filename[strlen(filename) - 4] = 0;
		strcat(filename, "Solved.bmp");
		FILE* fout = fopen(filename, "wb");

		Maze *maze = createMatFromBMPFile(imgdata);

		system("cls");
		maze->printMaze();

		vector<path_cell> path;	// the solution vector: <position: vec2, direction: int>
		if (solveBest(*maze, path)) {	// if there is a path
			writeBMPFileFromMat(maze, fout, path, imgdata);	// create a new image
		}

		fclose(fout);

		cout << "\nThe solution Image was created\n" <<
				"Try another image? y/n \n";
		if (_getch() != 'y')
			break;
	}

	delete[] filename;
	return 0;
}
