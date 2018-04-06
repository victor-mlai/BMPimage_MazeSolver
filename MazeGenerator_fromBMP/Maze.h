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
	int *maze;
	int nrRows;
	int nrCols;

public:
	Maze() {}
	Maze(int nrRows, int nrCols) : nrRows(nrRows), nrCols(nrCols) {
		maze = new int[nrRows * nrCols]();
	}

	~Maze() {
		delete[] maze;
	}

	inline int getVal(int row, int col) const {
		return maze[row * nrRows + col];
	}

	inline int getVal(const vec2 & v) const {
		return maze[v.x * nrRows + v.y];
	}

	void setVal(int row, int col, int value) {
		maze[row * nrRows + col] = value;
	}

	void setVal(const vec2 & v, int value) {
		maze[v.x * nrRows + v.y] = value;
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
};
