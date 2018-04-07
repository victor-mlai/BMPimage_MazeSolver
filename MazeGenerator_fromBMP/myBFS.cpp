#include "myBFS.h"
#include <queue>	// priority queue for bfs
#include <time.h>	// wait function

// stops the program for x seconds (used for delaying the output when printing the next arrow ( >>, <<, vv, ^^ ))
static void wait(float seconds)
{
	clock_t endwait = clock() + (clock_t)seconds * CLOCKS_PER_SEC;
	while (clock() < endwait) {}
}

// animates the printing of the maze's path
void printPath(Maze & maze, vector<path_cell> & path) {
	for (auto & it : path) {
		maze.setVal(it.position, it.direction);

		// comment here to show the maze directly solved
		// and to remove the stroboscopic effect
		//system("cls");
		//maze.printMazeCrop(it.position, minRadius);
		// wait(0.1f);
	}

	system("cls");
	maze.printMazeCrop(path.back().position, maxRadius);
}

// set the distance for each position to the closest exit
static void bfs(Maze & maze, const vector<vec2> & exits) {
	int n = maze.getNrRows();
	int m = maze.getNrCols();
	queue<vec2> q;

	// for each exit I apply Dijkstra to calculate the distances
	for (vec2 E : exits) {
		q.push(E);

		maze.setVal(E, DISTANCE_FROM_EXIT_TO_EXIT);	// marked the exit position

		while (!q.empty()) {
			vec2 F = q.front();
			q.pop();

			vec2 V;
			for (vec2 dir : directions) {	// for each neighbour
				V = F + dir;

				if (V.x >= 0 && V.x < n && V.y >= 0 && V.y < m) {
					if (maze.getVal(V) == 0 || maze.getVal(V) > maze.getVal(F) + 1) {
						maze.setVal(V, maze.getVal(F) + 1);
						q.push(V);
					}
				}
			}
		}

		// uncomment these to see the distances
		//printMat(maze);
		//_getch();
	}
}

// returns true if a solution exists
bool solveBest(Maze & maze, vector<path_cell> & path) {
	bool smallerValFound;
	path.reserve(maze.getNrCols() * maze.getNrRows());	// the maximum length the path can have

	vector<vec2> exits = readEndPos(maze);
	bfs(maze, exits);

	vec2 Start;	// Start Position
	while (true) {	// while I want to start from a different point
		readStartPos(maze, Start);
		path.push_back(path_cell(Start, 0));

		vec2 Curr = Start;	// Current Position
		vec2 Neigh;		// Neighbour's Position
		while (maze.getVal(Curr) != DISTANCE_FROM_EXIT_TO_EXIT) {	// while the Current position isn't the exit
			smallerValFound = false;
			for (int i = 0; i < 4; i++) {
				// for each neighbour
				Neigh = Curr + directions[i];
				// going to the neighbour that has the distance smaller than the current point
				if (maze.getVal(Neigh) < maze.getVal(Curr) && maze.getVal(Neigh) > 0) {
					smallerValFound = true;
					path.push_back(path_cell(Curr, i + 1));
					Curr = Neigh;
					break;
				}
			}
			if (!smallerValFound) {
				cout << "No solution exists\n";
				break;
			}
		}

		if (smallerValFound) {
			printPath(maze, path);
		}

		cout << "Try another starting point? y/n\n";
		if (_getch() == 'n')
			break;
	}

	return smallerValFound;
}
