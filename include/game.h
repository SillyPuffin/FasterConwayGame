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
	size_t getIndex(const int& x, const int& y); //return the index of the cell in a 1d array
	sizetPair getMapIndex(const size_t& index); //returns a size_t pair containing the uint64 chunk index and then the internal bit index of the cell inside that
	int boundaryLoop(int variable,const int& max);

	u8Pair getCell(const int& x, const int& y);
	uint8_t getState(const sizetPair& map_index);
	uint8_t getNeighbours(const int& x, const int& y, const sizetPair& map_index, const size_t& index);

	void setCell(const int& x,const  int& y,const uint8_t& state);
	void setNeighbours(const int& x, const int& y, const uint8_t& state, const bool& exclude_left); //mapindex, x , y, state, odd

	void clearCells();
	void randomiseCells();
	void getUserInput();
	void pause();
	void play();

	void update();

private:
	const int rows;
	const int columns;
	bool run = false;
	const unsigned int cellSize;

	const uint32_t BIT_CELL_SIZE = 4;
	const std::array<int8_t, 3> offset = { 1,0,-1 };
	bitMap cells;
	
};