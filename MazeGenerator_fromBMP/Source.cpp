#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	// scanf and printf
#include <time.h>	// wait function
#include <stdlib.h>	// malloc and calloc
#include <conio.h>	// _getch()
#include <vector>
#include <fstream>	// read write files
#include <iterator>	// iterate through files
using namespace std;

typedef struct
{
	int x, y;
}point;

void writeBMPFileFromMat(vector< vector<int> > mat, point st[500], int k, int *p, vector<unsigned char> imgdata) {
	int offset = imgdata[10];
	int width = imgdata[18];
	int height = imgdata[22];
	int bpp = imgdata[28] / 8;
	int padding = (4 - (width * bpp) % 4) % 4;

	int i;
	for (i = 1; i < k; i++) {
		mat[st[i].x][st[i].y] = 'x';
	}
	mat[st[i].x][st[i].y] = 'x';

	k = offset;
	for (int i = height; i >= 1; i--)
	{
		for (int j = 1; j <= width; j++)
		{
			int s = 0;
			for (int b = 0; b < bpp; b++)
				if (mat[i][j] == 'x')
					imgdata[k + b] /= 2;
			k += bpp;
		}
		k += padding;
	}

	unsigned char* vec = (unsigned char*)calloc(imgdata.size(), sizeof(unsigned char));
	int size = 0;
	for (char c : imgdata)
		vec[size++] = (unsigned char)c;
	FILE* f = fopen("img.bmp", "wb");
	fwrite(vec, 1, size, f);
	fclose(f);
}

void wait(float seconds)
{
	clock_t endwait = clock() + (clock_t)seconds * CLOCKS_PER_SEC;
	while (clock() < endwait) {}
}

int valid(vector< vector<int> > mat, point st[500], int k)
{
	int i;
	if (mat[st[k].x][st[k].y] == -1)
		return 0;
	for (i = 1; i<k; i++)
		if (st[k].x == st[i].x && st[k].y == st[i].y)
			return 0;
	return 1;
}

void afisare(vector< vector<int> > mat)
{
	int height = mat.size() - 1;
	int width = mat[0].size() - 1;
	int i, j;
	int zid = 254;
	printf("\n");
	for (j = 1; j <= width; j+=2)
		printf(" %-2d ", j);
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

void tipar(vector< vector<int> > mat, point st[500], int k, int *p, int height, int width)
{
	int i;
	for (i = 1; i<k; i++)
	{
		mat[st[i].x][st[i].y] = p[i + 1];
		
		//system("cls");
		//afisare(mat);
	}

	mat[st[i].x][st[i].y] = p[i];

	system("cls");
	afisare(mat);
}

point d[] = { { 0,-1 },{ 1,0 },{ 0,1 },{ -1,0 } };
void visit(vector< vector<int> >& mat, point E, int depth) {
	if (depth < 1)
		return;

	int n = mat.size() - 1;
	int m = mat[0].size() - 1;

	point V;
	for (int i = 0; i < 4; i++) {	// for each neighbour
		V.x = E.x + d[i].x;
		V.y = E.y + d[i].y;
		
		if (V.x > 0 && V.x < n && V.y > 0 && V.y < m) {
			if (mat[V.x][V.y] == 0 || mat[V.x][V.y] > mat[E.x][E.y] + 1) {
				mat[V.x][V.y] = mat[E.x][E.y] + 1;
				visit(mat, V, depth - 1);
			}
		}
	}
}

void printMat(vector< vector<int> > mat) {
	int n = mat.size() - 1;
	int m = mat[0].size() - 1;

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++) {
			printf("%-3d  ", mat[i][j]);
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
		printf("Try again\n");
		goto tryagain;
	}

	if (Sx == 1 || Sy == 1 || Sx == height || Sy == width)
	{
		printf("Look behind... the exit is right there\n");
		goto tryagain;
	}
}

void solveBest(vector< vector<int> > mat, FILE* fout, vector<unsigned char> imgdata) {
	int height = mat.size() - 1;
	int width = mat[1].size() - 1;
	
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
			printf("Try again\n");
			goto tryagain2;
		}

		exits.push_back({ Ex, Ey });
	}

	for (point E : exits) {
		mat[E.x][E.y] = 5;
		visit(mat, E, height*width / 4);
	}

	point* st = (point*)calloc((height*width / 2), sizeof(point));	// the solution vector
	int* p		= (int*)calloc((height*width / 2), sizeof(int));	// the direction
	if (!p || !st) { free(p);  free(st); }

	while (true) {
		int Sx, Sy;
		readStartPoz(mat, Sx, Sy);
		st[1].x = Sx;
		st[1].y = Sy;

		int Cx = Sx;
		int Cy = Sy;
		int Vx, Vy;
		int k = 1;
		int k2;
		while (mat[Cx][Cy] != 5) {
			k2 = k;
			for (int i = 0; i < 4; i++) {
				// for each neighbour
				Vx = Cx + d[i].x;
				Vy = Cy + d[i].y;
				if (mat[Vx][Vy] != -1 && mat[Vx][Vy] != 0 && mat[Vx][Vy] < mat[Cx][Cy]) {
					k++;
					Cx = Vx;
					Cy = Vy;
					st[k].x = Cx;
					st[k].y = Cy;
					p[k] = i + 1;
					break;
				}
			}
			if (k == k2) {
				printf("No solution exists\n");
				break;
			}
		}

		if (k != k2) {
			writeBMPFileFromMat(mat, st, k, p, imgdata);
			tipar(mat, st, k, p, height, width);
		}

		printf("Try another starting point? d/n\n");
		int d = _getch();
		if (d == 'n')
			break;
	}
}

void solveBKT(vector< vector<int> > mat, FILE* fout, vector<unsigned char> imgdata) {

	int height = mat.size() - 1;
	int width = mat[1].size() - 1;
	point *st;	// the solution vector
	int  *p;	// the direction
	st = (point*)calloc((height*width / 2), sizeof(point));
	p = (int*)calloc((height*width / 2), sizeof(int));
	if (!p || !st)
	{
		free(p);  free(st);
	}

	int Sx, Sy;
	readStartPoz(mat, Sx, Sy);
	st[1].x = Sx;
	st[1].y = Sy;

	int as, ev, ok = 0;
	point d[5] = { { 0,0 },{ 0,1 },{ 1,0 },{ 0,-1 },{ -1,0 } };

	int k = 2;
	while (k > 1)
	{
		as = 1; ev = 0;
		while (as != 0 && ev != 1)
		{
			if (p[k] < 4)
			{
				p[k]++;
				st[k].x = st[k - 1].x + d[p[k]].x;
				st[k].y = st[k - 1].y + d[p[k]].y;
				ev = valid(mat, st, k);
			}
			else
				as = 0;
		}
		if (as)
			if (st[k].x == 1 || st[k].x == height || st[k].y == 1 || st[k].y == width) {
				tipar(mat, st, k, p, height, width);
				ok = 1;
			}
			else {
				k++;
				p[k] = 0;
			}
		else
			k--;
	}

	if (!ok)
		printf("No solution exists\n");

	/* clear memory */
	mat.clear();
	free(p);  free(st);
}

vector< vector<int> > createMatFromBMPFile(vector<unsigned char> & imagedata) {
	unsigned char bpp, padding, offset;

	offset = imagedata[10];
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
	for (int i = height; i >= 1; i--)
	{
		for (int j = 1; j <= width; j++)
		{
			int s = 0;
			for (int b = 0; b < bpp; b++)
				s += (int)imagedata[k + b];
			if (s < 10 * bpp) // is pixel almost black
			{
				mat[i][j] = -1;	// put wall
			}
			k += bpp;
		}
		k += padding;
	}

	return mat;
}

int main()
{
	FILE* fout = fopen("maze41Solved.bmp", "wb");

	std::ifstream input("maze41.bmp", std::ios::binary);
	// copies all data into buffer
	std::vector<unsigned char> buffer((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));

	vector< vector<int> > mat = createMatFromBMPFile(buffer);
	afisare(mat);

	printf("d:Show every solution or just n:the best one? d/n\n");
	int d = _getch();

	if (d == 'd') {
		solveBKT(mat, fout, buffer);
	}
	else {
		solveBest(mat, fout, buffer);
	}

	printf("\nPress any key\n");
	_getch();	// waits any key
	
	return 0;
}
