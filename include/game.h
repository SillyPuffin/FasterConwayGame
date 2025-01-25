#pragma once

#include <iostream>

//my code
#include "structures.h"


class Game {

public:
	Game(unsigned int width, unsigned int height, unsigned int cellSize) : rows(height / cellSize), columns(width / cellSize), cellSize(cellSize) {
		size_t total_bits = rows * columns * BIT_CELL_SIZE;
		size_t num_uint64 = (total_bits + 63) / 64;
		cells.resize(num_uint64, 0);
	};

	void DrawBackground();
	void DrawCells();

	//cellmap functions
	size_t getIndex(const unsigned int& x, const unsigned int& y); //return the index of the cell in a 1d array
	sizetPair getMapIndex(const size_t& index); //returns a size_t pair containing the uint64 chunk index and then the internal bit index of the cell inside that

	u8Pair getCell(const unsigned int& x, const unsigned int& y);
	uint8_t getState(const sizetPair& map_index);
	uint8_t getNeighbours(const unsigned int& x, const unsigned int& y, const sizetPair& map_index, const size_t& index);

	void setCell(const unsigned int& x,const unsigned int& y,const uint8_t& state);

private:
	const unsigned int rows;
	const unsigned int columns;
	const unsigned int cellSize;

	const uint32_t BIT_CELL_SIZE = 4;
	bitMap cells;
};