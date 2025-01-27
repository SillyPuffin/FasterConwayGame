#pragma once

#include <iostream>
#include <mutex>

//my code
#include "structures.h"


class Game {

public:
	Game(unsigned int width, unsigned int height, int cellSize) : rows(height / cellSize), columns(width / cellSize), cellSize(cellSize) {
		size_t total_bits = rows * columns * BIT_CELL_SIZE;
		size_t num_uint64 = (total_bits + 63) / 64;
		cells.resize(num_uint64, 0);
		
		//defining the lookup table
		for (int state = 0; state < 2; state++) {
			for (int neighbourCount = 0; neighbourCount <= 8; neighbourCount++) {
				if (state==1) {
					if (neighbourCount >= 2 && neighbourCount <= 3) {
						lookup[state][neighbourCount] = 1;
					}
				}
				else {
					if (neighbourCount == 3) {
						lookup[state][neighbourCount] = 1;
					}
				}
			}
		}
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
	void changeNeighbourValue(const std::pair<int,int>& pos,const std::pair<int,int>& offsetPair, const uint8_t& state);

	void clearCells();
	void randomiseCells();
	void getUserInput();
	void pause();
	void play();

	void update();
	void stepSim();
	void genChangeList(const int& startRow, const int& rowSize, std::barrier<>* syncBarrier);
	

private:
	const int rows;
	const int columns;
	bool run = false;
	const int cellSize;

	const uint32_t BIT_CELL_SIZE = 4;
	const std::array<int, 3> offset = { 1,0,-1 };
	std::array<std::array<uint8_t, 9>, 2> lookup = { {{0}} }; //2d array for next state look up, in to format of 2 x 9 length vector
	std::mutex changeListMutex;
	bitMap cells;
	
	
};