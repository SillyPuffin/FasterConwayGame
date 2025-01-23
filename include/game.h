#pragma once

#include "structures.h"

class Game {

public:
	Game(int width, int height, int cellSize) : rows(height / cellSize), columns(height / cellSize), cellSize(cellSize) {
		size_t total_bits = rows * columns * BIT_CELL_SIZE;
		size_t num_uint64 = (total_bits + 63) / 64;
		cells.resize(num_uint64, 0);
	};

	void DrawBackground();
	void DrawCells();

	//cellmap functions
	sizetPair getIndex(int x, int y); //returns a size_t pair containing the uint64 chunk index and then the internal bit index of the cell inside that
	uint64_t getState(int x, int y);
	void setCell(int x, int y);

private:
	const unsigned int rows;
	const unsigned int columns;
	const unsigned int cellSize;

	const uint8_t BIT_CELL_SIZE = 4;
	bitMap cells;
};