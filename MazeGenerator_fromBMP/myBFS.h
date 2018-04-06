#pragma once
#include "IOutils.h"

static const int DISTANCE_FROM_EXIT_TO_EXIT = 5;
static const vec2 maxRadius = { 80, 240 };
static const vec2 minRadius = { 20, 20 };

bool solveBest(Maze & maze, vector<path_cell> & path);

void printPath(Maze & maze, vector<path_cell> & path);
