#pragma once

struct vec2 {
	int x, y;
	vec2() {}
	vec2(int x, int y) : x(x), y(y) {}
	vec2 operator+ (const vec2 & other) {
		return vec2(x + other.x, y + other.y);
	}
	void operator= (const vec2 & other) {
		x = other.x;
		y = other.y;
	}
};

class Maze
{
private:
	int **maze;
	int nrRows;
	int nrCols;

public:
	Maze() {}
	Maze(int nrRows, int nrCols) : nrRows(nrRows), nrCols(nrCols) {
		maze = new int*[nrRows];
		for (int i = 0; i < nrRows; i++) {
			maze[i] = new int[nrCols]();
		}
	}

	~Maze() {
		for (int i = 0; i < nrRows; i++) {
			delete[] maze[i];
		}
		delete[] maze;
	}

	inline int getVal(int row, int col) const {
		return maze[row][col];
	}

	inline int getVal(const vec2 & v) const {
		return maze[v.x][v.y];
	}

	void setVal(int row, int col, int value) {
		maze[row][col] = value;
	}

	void setVal(const vec2 & v, int value) {
		maze[v.x][v.y] = value;
	}

	inline int getNrRows() const {
		return nrRows;
	}

	inline int getNrCols() const {
		return nrCols;
	}

	// prints a crop of the maze
	void printMazeCrop(const vec2 & center, const vec2 & radius) const;

	// prints the entire maze
	void printMaze();

	static const int BLOCK = -1;
	static const int EMPTY = 0;
	static const int N = 1;
	static const int E = 2;
	static const int S = 3;
	static const int W = 4;
};
