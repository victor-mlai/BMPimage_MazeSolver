#include "Maze.h"

#include <stdio.h>

// prints a crop of the maze
void Maze::printMazeCrop(const vec2 & center, const vec2 & radius) const {
	int zid = 219;

	vec2 upLeftCorner = { center.x - radius.x < 1 ? 0 : center.x - radius.x,
		center.y - radius.y < 1 ? 0 : center.y - radius.y };
	vec2 bottomRightCorner = { center.x + radius.x > nrRows ? nrRows - 1 : center.x + radius.x,
		center.y + radius.y > nrCols ? nrCols - 1 : center.y + radius.y };

	printf("\n");

	for (int j = upLeftCorner.y; j <= bottomRightCorner.y; j += 2)
		printf("%-2d  ", j);

	printf("\n");

	for (int i = upLeftCorner.x; i <= bottomRightCorner.x; i++) {
		for (int j = upLeftCorner.y; j <= bottomRightCorner.y; j++) {
			switch (this->getVal(i, j)) {
			case -1: printf("%c%c", zid, zid); break;
			case 1:  printf("<<"); break;
			case 2:  printf("vv"); break;
			case 3:  printf(">>"); break;
			case 4:  printf("^^"); break;
			default: printf("  "); break;
			}
		}
		printf("%d\n", i);
	}

	printf("\n");
}

// prints the entire maze
void Maze::printMaze() {
	vec2 center{ nrRows / 2 , nrCols / 2 };
	vec2 radius{ nrRows / 2 + 1 , nrCols / 2 + 1 };

	// radius is big enough, so it prints the entire maze
	this->printMazeCrop(center, radius);
}
