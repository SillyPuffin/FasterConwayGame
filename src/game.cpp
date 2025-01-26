#pragma once

#include <raylib.h>
#include <iostream>
#include <string>

//my code
#include "game.h"
#include "structures.h"

void Game::DrawBackground() {
	ClearBackground(Color{ 50, 50, 50, 255 });
	Color Black = { 0,0,0,255 };

	for (int column = 1; column < columns+1; column++) {
		int x = column * cellSize;
		DrawLine(x, 0, x, columns * cellSize, Black);
	}

	for (int row = 1; row < rows+1; row++) {
		int y = row * cellSize - 1;
		DrawLine(0,y, cellSize*columns,y, Black);
	}

}

void Game::DrawCells() {
	for (int x = 0; x < columns; x++) {
		for (int y = 0; y < rows; y++) {
			size_t index = getIndex(x, y);
			sizetPair map_idx = getMapIndex(index);

			uint64_t state = getState(map_idx);

			if (state == 1) {
				DrawRectangle(x * cellSize, y * cellSize, cellSize-1, cellSize-1, Color{ 0,255,0,255 });
			}
		}
	}
}

size_t Game::getIndex(const int& x, const int& y) {
	return size_t(y * columns + x);
}

sizetPair Game::getMapIndex(const size_t& index) {
	size_t bit_index = index * BIT_CELL_SIZE;
	size_t chunk_index = bit_index / 64;
	size_t chunk_offset = bit_index % 64;

	return sizetPair{ chunk_index, chunk_offset };
}

int Game::boundaryLoop(int variable, const int& max) {
	if (variable >= max) {
		return { 0 };
	}
	else if (variable < 0) {
		return { max - 1 };
	}
	else {
		return variable;
	}

}

void Game::setCell(const int& x, const int& y, const uint8_t& state) {
	uint64_t index = getIndex(x, y);
	sizetPair mapIndex = getMapIndex(index);

	//if setting alive
	if (state) {
		cells[mapIndex.first] |= (1ULL << mapIndex.second);
		if (index % 2 == 0) {
			setNeighbours(x, y, state, false);
		}
		else {
			setNeighbours(x, y, state, true);
		}
	} 
	else {
		cells[mapIndex.first] &= ~(1ULL << mapIndex.second);
		if (index % 2 == 0) {
			setNeighbours(x, y, state, false);
		}
		else {
			setNeighbours(x, y, state, true);
		}
	}

}

void Game::setNeighbours(const int& x, const int& y, const uint8_t& state, const bool& exclude_left) {
	//if even exclude right, if odd exclude left
	for (const int8_t& dx : offset) {
		for (const int8_t& dy : offset) {
			if (dy != 0) {
				int new_x = boundaryLoop(x + dx, columns);
				int new_y = boundaryLoop(y + dy, rows);
				sizetPair map_idx = getMapIndex(getIndex(new_x, new_y));
				uint64_t value = (cells[map_idx.first] >> (map_idx.second + 1)) & ((1ULL << (BIT_CELL_SIZE - 1)) - 1);

				if (state == 1) {//add to the neighbours
					value += 1;
					cells[map_idx.first] &= ~(((1ULL << (BIT_CELL_SIZE - 1)) - 1) << (map_idx.second+1)); //clear the bits at that location
					cells[map_idx.first] |= value << (map_idx.second + 1);
				}
				else if (state == 0) {
					value -= 1;
					cells[map_idx.first] &= ~(((1ULL << (BIT_CELL_SIZE - 1)) - 1) << map_idx.second); //clear the bits at that location
					cells[map_idx.first] |= value << (map_idx.second + 1);
				}
			}
			else if (dy == 0 && dx != 0) {
				if (exclude_left) {
					if (dx != -1) {
						//gonna be horrednous looking but were going to check state then if its a 1 add a neighbour count to all surrounding cells
						int new_x = boundaryLoop(x + dx, columns);
						int new_y = boundaryLoop(y + dy, rows);
						sizetPair map_idx = getMapIndex(getIndex(new_x, new_y));
						uint64_t value = (cells[map_idx.first] >> (map_idx.second + 1)) & ((1ULL << (BIT_CELL_SIZE - 1)) - 1);

						if (state == 1) {//add to the neighbours
							value += 1;
							cells[map_idx.first] &= ~(((1ULL << (BIT_CELL_SIZE - 1)) - 1) << (map_idx.second + 1)); //clear the bits at that location
							cells[map_idx.first] |= value << (map_idx.second + 1);
						}
						else if (state == 0) {
							value -= 1;
							cells[map_idx.first] &= ~(((1ULL << (BIT_CELL_SIZE - 1)) - 1) << map_idx.second); //clear the bits at that location
							cells[map_idx.first] |= value << (map_idx.second + 1);
						}
					}
				}
				else {
					if (dx != 1) {
						int new_x = boundaryLoop(x + dx, columns);
						int new_y = boundaryLoop(y + dy, rows);
						sizetPair map_idx = getMapIndex(getIndex(new_x, new_y));
						uint64_t value = (cells[map_idx.first] >> (map_idx.second + 1)) & ((1ULL << (BIT_CELL_SIZE - 1)) - 1);

						if (state == 1) {//add to the neighbours
							value += 1;
							cells[map_idx.first] &= ~(((1ULL << (BIT_CELL_SIZE - 1)) - 1) << (map_idx.second + 1)); //clear the bits at that location
							cells[map_idx.first] |= value << (map_idx.second + 1);
						}
						else if (state == 0) {
							value -= 1;
							cells[map_idx.first] &= ~(((1ULL << (BIT_CELL_SIZE - 1)) - 1) << map_idx.second); //clear the bits at that location
							cells[map_idx.first] |= value << (map_idx.second + 1);
						}
					}
				}
			}
		}
	}
}

u8Pair Game::getCell(const int& x, const int& y) {
	size_t index = getIndex(x, y);
	sizetPair map_idx = getMapIndex(index);

	uint8_t neighbours = getNeighbours(x, y, map_idx, index);
	uint8_t state = getState(map_idx);

	return { neighbours, state };
}

uint8_t Game::getState(const sizetPair& mapIndex) {
	uint8_t state = 0b1 & (cells[mapIndex.first] >> mapIndex.second);

	return state;
}

uint8_t Game::getNeighbours(const int& x, const int& y,const sizetPair& map_index, const size_t& index) {
	//this line looks a bit silly but all it does is bit shift the chunk down by whatever the cell bit offset is + 1 to get the three neighbour bits and not the state
	//then it masks it with 0b111 to get just the neighbours, then cast to a uint8_t because the max neighbour size is 7
	uint8_t value = cells[map_index.first] >> (map_index.second + 1) & (uint64_t(1) << (BIT_CELL_SIZE - 1)) - 1;
	

	//std::cout << columns << " " << 0 % 2 << " "  << getMapIndex({index + 1}).first << "\n";
	//if even go to the right for the missing neighbour value
	if (index % 2 == 0) {
		//check range of index and then wrap if larger than max width
		if (x + 1 == columns) {
			sizetPair new_mapIndex = getMapIndex(static_cast<size_t>(y*columns));
			value += getState(new_mapIndex);
		}
		else {
			sizetPair new_mapIndex = getMapIndex({ index + 1 });
			value += getState(new_mapIndex);
		}
	}
	else { //if odd go to the left
		if (x - 1 < 0) {
			sizetPair new_mapIndex = getMapIndex(static_cast<size_t>(y * columns + columns));
			value += getState(new_mapIndex);
		}
		else {
			sizetPair new_mapIndex = getMapIndex({ index - 1 });
			value += getState(new_mapIndex);
		}
	}

	return value;
} 

void Game::pause() {
	run = false;
}

void Game::play() {
	run = true;
}

void Game::clearCells() {
	std::fill(cells.begin(), cells.end(), 0);
}

void Game::randomiseCells() {
	clearCells();

	//random chance of 0.2 to set a cell to true
	for (int x = 0; x < columns; x++) {
		for (int y = 0; y < rows; y++) {
			size_t index = getIndex(x, y);
			sizetPair map_idx = getMapIndex(index);

			int random = GetRandomValue(0, 4);
			if (random == 4) {
				setCell(x, y, 1);
			}
		}
	}
}