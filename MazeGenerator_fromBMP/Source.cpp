#define _CRT_SECURE_NO_WARNINGS

#include "IOutils.h"
#include "myBFS.h"
#include <iterator>	// std::istreambuf_iterator
#include <filesystem>

namespace fs = std::experimental::filesystem;

#define INPUT_DIRECTORY "../input_images/"
#define OUTPUT_DIRECTORY "../output_images/"

std::string getInputFilePath(std::string &filename) {
	std::string dir = INPUT_DIRECTORY;

	unsigned int i = 0;
	std::cout << "id) <path>\n";
	for (auto & p : fs::directory_iterator(dir)) {
		std::cout << i++ << ") " << p.path().filename().string() << std::endl;
	}

	if (i == 0) {
		std::cout << "Input Folder is empty or doesn't exist\n" <<
			"Exiting . . .\n";
		return "";
	}

	unsigned char c;
	do {
		std::cout << "Choose input BMP file id [0 - " << i-1 << "]\n";
		c = _getch();
		c -= '0';
	} while (c >= i);

	i = 0;
	std::cout << "Choose input BMP file name:\n";
	for (auto & p : fs::directory_iterator(dir)) {
		if (i++ == c) {
			filename = p.path().filename().string();
			return p.path().string();
		}
	}

	return "";
}

void getFileName(char* filename) {
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
}

// copies all data from BMP file into imgdata
void getImgData(std::vector<unsigned char> &imgdata, std::string &inpFile) {
	std::ifstream input(inpFile, std::ios::binary);

	imgdata = std::vector<unsigned char>((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	input.close();
}

int main() {
	for(;;) {
		std::string filename;
		std::string inpFilePath = getInputFilePath(filename);
		if (inpFilePath == "")
			return -1;

		std::vector<unsigned char> imgdata;
		getImgData(imgdata, inpFilePath);

		Maze *maze = createMatFromBMPFile(imgdata);

		system("cls");
		maze->printMaze();

		vector<path_cell> path;	// the solution vector: <position: vec2, direction: int>
		if (solveBest(*maze, path)) {	// if there is a path
			// create output file
			std::string outFilePath = OUTPUT_DIRECTORY;
			outFilePath.append(filename);
			outFilePath.erase(outFilePath.length() - 4);	// erase ".bmp"
			outFilePath.append("Solved.bmp");

			FILE* fout = fopen(outFilePath.c_str(), "wb");

			// write in the output file
			writeBMPFileFromMat(maze, fout, path, imgdata);
			
			fclose(fout);

			std::cout << "\nThe solution Image was created\n";
		}

		std::cout << "Try another image? y/n \n";
		if (_getch() != 'y')
			break;

		delete maze;
	}

	return 0;
}
