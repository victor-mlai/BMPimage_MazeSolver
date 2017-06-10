#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	// scanf and printf
#include <time.h>	// wait function
#include <stdlib.h>	// malloc and calloc
#include <conio.h>	// _getch()
#include <vector>
using namespace std;

typedef struct
{
	int x, y;
}point;

void wait(float seconds)
{
	clock_t endwait = clock() + (clock_t)seconds * CLOCKS_PER_SEC;
	while (clock() < endwait) {}
}

int valid(vector< vector<int> > mat, point st[500], int k)
{
	int i;
	if (mat[st[k].x][st[k].y] == 1)
		return 0;
	for (i = 1; i<k; i++)
		if (st[k].x == st[i].x && st[k].y == st[i].y)
			return 0;
	return 1;
}

void afisare(vector< vector<int> > mat, int height, int width)
{
	int i, j;
	static int zid = 254, sus = 94, jos = 118, stg = 60, dr = 62;
	printf("\n");
	for (j = 1; j <= width; j++)
		printf(" %-2d", j);
	printf("\n");
	for (i = 1; i <= height; i++)
	{
		for (j = 1; j <= width; j++)
			switch (mat[i][j])
			{
			case 0: printf("   "); break;
			case 1: printf("%c%c%c", zid, zid, zid); break;
			case 2: printf(" %c ", jos); break;
			case 3: printf(" %c ", stg); break;
			case 4: printf(" %c ", sus); break;
			case 5: printf(" %c ", dr); break;
			default: break;
			}
		printf(" %d\n", i);
	}
	printf("\n");
}

void tipar(vector< vector<int> > mat, point st[500], int k, int *ok, int *p, int height, int width)
{
	int i;
	*ok = 1;

	for (i = 1; i<k; i++)
	{
		switch (p[i + 1])
		{
		case 1:  mat[st[i].x][st[i].y] = 5;  break;
		default: mat[st[i].x][st[i].y] = p[i + 1];  break;
		}
		system("cls");
		afisare(mat, height, width);
	}

	switch (p[i])
	{
	case 1:  mat[st[i].x][st[i].y] = 5;  break;
	default: mat[st[i].x][st[i].y] = p[i];  break;
	}

	system("cls");
	afisare(mat, height, width);
}

int main()
{
	vector<int> imagedata;

	int bytval, i, j, height, width, padding, offset, bbs;

	FILE *fr = fopen("maze2.bmp", "rb");
	if (!fr)	return 0;

	do  /*citim img intr-un vector */
	{
		bytval = fgetc(fr);
		imagedata.push_back(bytval);
	} while (bytval != EOF);

	fclose(fr);

	width = imagedata[18];
	height = imagedata[22];
	padding = (4 - (width * 3) % 4) % 4;
	offset = imagedata[10];
	bbs = imagedata[28];	// 3 or 4 (RGB or RGBA)

	/* creare matrice */

	vector< vector<int> > mat(height + 1, vector<int>(width + 1));

	int k = offset;
	for (i = height; i >= 1; i--)
	{
		for (j = 1; j <= width; j++)
		{
			if (imagedata[k] + imagedata[k + 1] + imagedata[k + 2] < 100)
			{
				mat[i][j] = 1;
			}
			k += bbs;
		}
		k += padding;
		afisare(mat, height, width);
	}
	imagedata.clear();

	/* citire pozitie inceput */

	point *st;
	int  *p;
	st = (point*)calloc((height*width / 2), sizeof(point));
	p =    (int*)calloc((height*width / 2), sizeof(int));
	if (!p || !st)
	{
		free(p);  free(st);
	}

	afisare(mat, height, width);

tryagain:
	scanf("%d%d", &st[1].x, &st[1].y);

	if (st[1].x < 1 || st[1].x > height || st[1].y < 1 || st[1].y > width || mat[st[1].x][st[1].y] == 1)
	{
		printf("Mai incearca odata\n");
		goto tryagain;
	}
	if (st[1].x == 1 || st[1].y == 1 || st[1].x == height || st[1].y == width)
	{
		printf("Look behind... the exit is right there\n");
		goto tryagain;
	}

	//bt();

	int as, ev, ok = 0;
	point d[5] = { { 0,0 },{ 0,1 },{ 1,0 },{ 0,-1 },{ -1,0 } };

	k = 2;
	while (k>1)
	{
		as = 1; ev = 0;
		while (as != 0 && ev != 1)
		{
			if (p[k]<4)
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
			if (st[k].x == 1 || st[k].x == height || st[k].y == 1 || st[k].y == width)
				tipar(mat, st, k, &ok, p, height, width);
			else
			{
				k++;
				p[k] = 0;
			}
		else k--;
	}

	if (!ok)
		printf("Nu exista solutii\n");

	/* eliberare memorie alocata */
	mat.clear();
	free(p);  free(st);
	return 0;
}
