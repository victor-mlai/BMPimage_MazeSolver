#pragma once
#include "Maze.h"
#include <stdio.h>	// printf
#include <conio.h>	// _getch()
#include <vector>
#include <fstream>	// read input file
#include <iostream>	// cin & cout

using namespace std;

static const vec2 directions[] = { { -1, 0 },{ 0, 1 },{ 1, 0 },{ 0, -1 } };	// Up, Rigt, Down, Left
static const int PATH_ENCODING = -2;

struct path_cell {
	vec2 position;
	int direction;
	path_cell(vec2 &position, int direction) : position(position), direction(direction) {}
};

Maze* createMatFromBMPFile(vector<unsigned char> & imagedata);

void writeBMPFileFromMat(Maze *maze, FILE* fout,
	vector<path_cell> & path,
	vector<unsigned char> & imgdata);

void printMat(const Maze & maze);

void readWhatToCrop(const Maze & maze);

void readStartPos(const Maze & maze, vec2 & Start);

vector<vec2> readEndPos(const Maze & maze);