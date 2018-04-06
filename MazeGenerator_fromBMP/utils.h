#pragma once
#include "Maze.h"
#include <stdio.h>	// printf
#include <time.h>	// wait function
#include <conio.h>	// _getch()
#include <vector>
#include <queue>	// priority queue for bfs
#include <fstream>	// read input file
#include <iterator>	// iterate through file's bits
#include <iostream>	// cin & cout

using namespace std;


Maze* createMatFromBMPFile(vector<unsigned char> & imagedata);

void writeBMPFileFromMat(Maze *maze, FILE* fout,
	vector< pair<vec2, int> > & path,	// position, direction
	vector<unsigned char> & imgdata);

void printMat(Maze & maze);

void readWhatToCrop(const Maze & maze);

void readStartPos(Maze & maze, vec2 & Start);

vector<vec2> readEndPos(const Maze & maze);