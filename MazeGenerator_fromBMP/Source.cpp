#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	// scanf and printf
#include <time.h>	// wait function
#include <stdlib.h>	// malloc and calloc
#include <conio.h>	// _getch()
#include <vector>
#include <queue>	// priority queue for bfs
#include <fstream>	// read input file
#include <iterator>	// iterate through file's bits
using namespace std;

struct point{ int x, y; };

point d[] = { { 0,-1 },{ 1,0 },{ 0,1 },{ -1,0 } };	// used by solveBest and solveBKT for calculating the neighbour point

vector< vector<int> > createMatFromBMPFile(vector<unsigned char> imagedata) {
	unsigned char bpp, padding;

	uint16_t offset = imagedata[11];
	offset <<= 8;
	offset += imagedata[10];

	uint16_t width = imagedata[19];
	width <<= 8;
	width += imagedata[18];

	uint16_t height = imagedata[23];
	height <<= 8;
	height += imagedata[22];

	bpp = imagedata[28] / 8;	// bytes per pixel (bits/8 = bytes)
	padding = (4 - (width * bpp) % 4) % 4;

	/* creare matrice */
	vector< vector<int> > mat(height + 1, vector<int>(width + 1));

	int k = offset;
	for (int i = height; i >= 1; i--) {
		for (int j = 1; j <= width; j++) {
			int s = 0;
			for (int b = 0; b < bpp; b++) {
				s += (int)imagedata[k + b];
			}
			if (s < 240 * bpp) { // is pixel almost black
				mat[i][j] = -1;	// put wall
			}
			k += bpp;
		}
		k += padding;
	}

	return mat;
}

void writeBMPFileFromMat(vector< vector<int> > mat, FILE* fout, vector<point> sol, int k, vector<int> dir, vector<unsigned char> imgdata) {
	unsigned char bpp, padding;

	uint16_t offset = imgdata[11];
	offset <<= 8;
	offset += imgdata[10];

	uint16_t width = imgdata[19];
	width <<= 8;
	width += imgdata[18];

	uint16_t height = imgdata[23];
	height <<= 8;
	height += imgdata[22];

	bpp = imgdata[28] / 8;	// bytes per pixel (bits/8 = bytes)
	padding = (4 - (width * bpp) % 4) % 4;

	// marking the path to colour it differently
	int i;
	for (i = 1; i < k; i++) {
		mat[sol[i].x][sol[i].y] = 'x';
	}
	mat[sol[i].x][sol[i].y] = 'x';

	// rewriting the imgdata with the path coloured
	k = offset;
	for (int i = height; i >= 1; i--) {
		for (int j = 1; j <= width; j++) {
			int s = 0;
			for (int b = 0; b < bpp; b++)
				if (mat[i][j] == 'x')
					imgdata[k + b] /= 2;
			k += bpp;
		}
		k += padding;
	}

	// create auxiliary vector so I can call fwrite with the right parameters
	unsigned char* vec = (unsigned char*)calloc(imgdata.size(), sizeof(unsigned char));
	int size = 0;
	for (char c : imgdata)
		vec[size++] = (unsigned char)c;

	fwrite(vec, 1, size, fout);
	free(vec);
}

// stops the program for x seconds (used for delaying the output when printing the next arrow ( >>, <<, vv, ^^ ))
void wait(float seconds)
{
	clock_t endwait = clock() + (clock_t)seconds * CLOCKS_PER_SEC;
	while (clock() < endwait) {}
}

// prints the maze with all the distances (used for debugging in bfs)
void printMat(vector< vector<int> > mat) {
	int n = mat.size() - 1;
	int m = mat[0].size() - 1;
	int zid = 254;

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++) {
			if (mat[i][j] == -1)
				printf("%c%c%c%c", zid, zid, zid, zid);
			else
				printf("%-3d ", mat[i][j]);
		}
		printf("\n");
	}
}

void readStartPoz(vector< vector<int> > mat, int& Sx, int& Sy) {
	int height = mat.size() - 1;
	int width = mat[0].size() - 1;

tryagain:
	printf("Write the coord for start position (Format: x y)\n");
	scanf("%d %d", &Sx, &Sy);

	if (Sx < 1 || Sx > height || Sy < 1 || Sy > width || mat[Sx][Sy] == -1)
	{
		printf("Outside or Wall => Try again\n");
		goto tryagain;
	}
}

vector<point> readEndPoz(vector< vector<int> > mat) {
	int height = mat.size();
	int width = mat[0].size();

	int nr_exits;
	printf("How many exits?\n");
	scanf("%d", &nr_exits);

	int Ex, Ey;	// End coordonates
	vector<point> exits;
	for (int i = 0; i < nr_exits; i++) {
	tryagain2:
		printf("Write the coord for end position (Format: x y)\n");
		scanf("%d %d", &Ex, &Ey);

		if (Ex < 1 || Ex > height || Ey < 1 || Ey > width || mat[Ex][Ey] == -1)
		{
			printf("Outside or Wall => Try again\n");
			goto tryagain2;
		}

		exits.push_back({ Ex, Ey });
	}

	return exits;
}

// prints the entire maze
void printMaze(vector< vector<int> > mat) {
	int height = mat.size() - 1;
	int width = mat[0].size() - 1;
	int i, j;
	int zid = 254;
	printf("\n");
	for (j = 1; j <= width; j+=2)
		printf("%-2d  ", j);
	printf("\n");
	for (i = 1; i <= height; i++)
	{
		for (j = 1; j <= width; j++)
			switch (mat[i][j])
			{
			case -1: printf("%c%c", zid, zid); break;
			case 1: printf("<<"); break;
			case 2: printf("vv"); break;
			case 3: printf(">>"); break;
			case 4: printf("^^"); break;
			default: printf("  "); break;
			}
		printf(" %d\n", i);
	}
	printf("\n");
}

// prints a crop of the maze
void printMazeCrop(vector< vector<int> > mat, point center, int radius) {
	int height = mat.size() - 1;
	int width = mat[0].size() - 1;
	int i, j;
	int zid = 254;
	printf("\n");

	point start = { center.x - radius < 1 ? 1 : center.x - radius,center.y - radius < 1 ? 1 : center.y - radius };
	point end = { center.x + radius > height ? height : center.x + radius,center.y + radius > width ? width : center.y + radius };

	for (j = start.y; j <= end.y; j += 2)
		printf("%-2d  ", j);
	printf("\n");
	for (i = start.x; i <= end.x; i++)
	{
		for (j = start.y; j <= end.y; j++)
			switch (mat[i][j])
			{
			case -1: printf("%c%c", zid, zid); break;
			case 1: printf("<<"); break;
			case 2: printf("vv"); break;
			case 3: printf(">>"); break;
			case 4: printf("^^"); break;
			default: printf("  "); break;
			}
		printf(" %d\n", i);
	}
	printf("\n");
}

// animates the printing of the maze's path
void tipar(vector< vector<int> > mat, vector<point> sol, int k, vector<int> dir) {
	int i;
	for (i = 1; i<k; i++)
	{
		mat[sol[i].x][sol[i].y] = dir[i + 1];

		// comment here to show the maze directly solved
		system("cls");
		printMazeCrop(mat, sol[i], 20);
	}

	mat[sol[i].x][sol[i].y] = dir[i];

	system("cls");
	printMazeCrop(mat, sol[i], 20);
}

// for every point in the maze assigns the smallest distance to any exit
void bfs(vector< vector<int> >& mat, vector<point> exits) {
	int n = mat.size() - 1;
	int m = mat[0].size() - 1;
	queue<point> q;

	// for each exit point I apply Dijkstra to calculate the distances
	for (point E : exits) {
		q.push(E);
		mat[E.x][E.y] = 5;	// marked the end position

		while (!q.empty()) {
			point F = q.front();
			q.pop();

			point V;
			for (int i = 0; i < 4; i++) {	// for each neighbour
				V.x = F.x + d[i].x;
				V.y = F.y + d[i].y;

				if (V.x > 0 && V.x < n && V.y > 0 && V.y < m) {
					if (mat[V.x][V.y] == 0 || mat[V.x][V.y] > mat[F.x][F.y] + 1) {
						mat[V.x][V.y] = mat[F.x][F.y] + 1;
						q.push(V);
					}
				}
			}
		}

		//printMat(mat); // uncomment this to see the distances
		//_getch();
	}
}

void solveBest(vector< vector<int> > mat, FILE* fout, vector<unsigned char> imgdata) {
	int height = mat.size() - 1;
	int width = mat[1].size() - 1;
	
	vector<point> exits = readEndPoz(mat);

	bfs(mat, exits);

	vector<point> sol(height*width / 2);	// the solution vector
	vector<int> dir(height*width / 2);	// the directions

	// while I want to start from a different point
	int Sx, Sy;
	while (true) {
		readStartPoz(mat, Sx, Sy);
		sol[1].x = Sx;
		sol[1].y = Sy;

		int Cx = Sx;
		int Cy = Sy;
		int Vx, Vy;
		int k = 1;
		int k2;
		while (mat[Cx][Cy] != 5) {	// while the Current position isn't the exit
			k2 = k;	// for future checks
			for (int i = 0; i < 4; i++) {
				// for each neighbour
				Vx = Cx + d[i].x;
				Vy = Cy + d[i].y;
				// going to the neighbour that has the distance smaller than the current point
				if (mat[Vx][Vy] != -1 && mat[Vx][Vy] != 0 && mat[Vx][Vy] < mat[Cx][Cy]) {
					k++;
					Cx = Vx;
					Cy = Vy;
					sol[k].x = Cx;
					sol[k].y = Cy;
					dir[k] = i + 1;
					break;
				}
			}
			if (k == k2) {
				printf("No solution exists\n");
				break;
			}
		}

		if (k != k2) {
			writeBMPFileFromMat(mat, fout, sol, k, dir, imgdata);
			tipar(mat, sol, k, dir);
		}

		printf("Try another starting point? y/n\n");
		if (_getch() == 'n')
			break;

		printMazeCrop(mat, { 50,50 }, 60);
	}

	/* clear memory */
	mat.clear();
	dir.clear();
	sol.clear();
	exits.clear();
}

// checks if the solution is valid
bool valid(vector< vector<int> > mat, vector<point> sol, int k)
{
	int i;
	if (mat[sol[k].x][sol[k].y] == -1)	// if it's a wall on this position
		return false;

	// check if the solution already has this position
	for (i = 1; i < k; i++)
		if (sol[k].x == sol[i].x && sol[k].y == sol[i].y)
			return false;

	return true;
}

// Backtracking
void solveBKT(vector< vector<int> > mat, FILE* fout, vector<unsigned char> imgdata) {
	int height = mat.size() - 1;
	int width = mat[1].size() - 1;

	vector<point> sol(height*width / 2);	// the solution vector
	vector<int> dir(height*width / 2);	// the directions

	vector<point> exits = readEndPoz(mat); 
	
	int Sx, Sy;
	readStartPoz(mat, Sx, Sy);
	sol[1].x = Sx;
	sol[1].y = Sy;

	printf("Calculating...\n");
	bool isSol = false;

	for (point E : exits) {
		mat[E.x][E.y] = 5;	// marked the exits
	}

	int k = 2;
	while (k > 1) {	// while there are still solutions to find
		while (dir[k] < 5) {	// for each direction
			sol[k].x = sol[k - 1].x + d[dir[k]].x;
			sol[k].y = sol[k - 1].y + d[dir[k]].y;
			dir[k]++;
			if (valid(mat, sol, k))	// check if it's a valid position for the solution
				break;
		}
		if (dir[k] < 5) {
			if (mat[sol[k].x][sol[k].y] == 5) {	// if it's an exit on this position
				tipar(mat, sol, k, dir);
				writeBMPFileFromMat(mat, fout, sol, k, dir, imgdata);
				isSol = true;	// a solution was found

				printf("\nStop at this solution? y/n\n");
				if (_getch() == 'y')
					break;
			}
			else {
				k++;
				dir[k] = 0;
			}
		}
		else {
			k--;	// go back, I ended in a dead-end
		}
	}

	if (!isSol)
		printf("No solution exists\n");

	/* clear memory */
	mat.clear();
	dir.clear();
	sol.clear();
	exits.clear();
}

int main() {
	char* filename = (char*)malloc(20);
	printf("Choose input BMP file name:\n 1)maze.bmp\n 2)maze2.bmp\n 3)maze3.bmp\n 4)1024.bmp\n x)Other\n");
	switch (_getch()) {
	case '1': strcpy(filename, "maze1.bmp"); printf("Recommended\n end point: 2 31\n start point: 30 2\n"); break;
	case '2': strcpy(filename, "maze2.bmp"); printf("Recommended\n end point: 57 1\n start point: 14 21\n"); break;
	case '3': strcpy(filename, "maze3.bmp"); printf("Recommended\n end point: 74 94\n start point: 10 10\n"); break;
	case '4': strcpy(filename, "1024.bmp"); printf("Recommended\n end point: 1022 1024\n start point: 1 3\n"); break;
	default: printf("Please Enter a file name: "); scanf("%s", filename); break;
	}

	printf("\nPress any key to continue\n");
	_getch();	// waits any key

	// input file
	std::ifstream input(filename, std::ios::binary);
	// copies all data into buffer
	std::vector<unsigned char> buffer((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));
	input.close();

	// output file
	filename[strlen(filename) - 4] = 0;
	strcat(filename, "Solved.bmp");
	FILE* fout = fopen(filename, "wb");

	vector< vector<int> > mat = createMatFromBMPFile(buffer);
	printMazeCrop(mat, { 50,50 }, 60);

	printf("y:Show the best solution (Recomended) or\nn:every single solution? (Note that Backtracking is very slow)\n y/n\n");
	if (_getch() == 'y') {
		solveBest(mat, fout, buffer);
	}
	else {
		solveBKT(mat, fout, buffer);
	}

	fclose(fout);
	printf("\nThe solution Image was created\nPress any key to exit\n");
	_getch();	// waits any key

	// free memory
	buffer.clear();
	mat.clear();

	return 0;
}
