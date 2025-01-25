#pragma once

#include <raylib.h>
#include <iostream>
#include <string>

//my code
#include "game.h"

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
	for (unsigned int x = 0; x < columns; x++) {
		for (unsigned int y = 0; y < rows; y++) {
			size_t index = getIndex(x, y);
			sizetPair map_idx = getMapIndex(index);

			uint64_t state = getState(map_idx);

			if (state == 1) {
				DrawRectangle(x * cellSize, y * cellSize, cellSize-1, cellSize-1, Color{ 0,255,0,255 });
			}
		}
	}
}

size_t Game::getIndex(const unsigned int& x, const unsigned int& y) {
	return size_t(y * columns + x);
}

sizetPair Game::getMapIndex(const size_t& index) {
	size_t bit_index = index * BIT_CELL_SIZE;
	size_t chunk_index = bit_index / 64;
	size_t chunk_offset = bit_index % 64;

	return sizetPair{ chunk_index, chunk_offset };
}

void Game::setCell(const unsigned int& x, const unsigned int& y, const uint8_t& state) {
	sizetPair mapIndex = getMapIndex(getIndex(x, y));

	//if setting alive
	if (state) {
		cells[mapIndex.first] |= (1ULL << mapIndex.second);
		//set neighbours
	} 
	else {
		cells[mapIndex.first] &= ~(1ULL << mapIndex.second);
		//set neighbours
	}

}

u8Pair Game::getCell(const unsigned int& x, const unsigned int& y) {
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

uint8_t Game::getNeighbours(const unsigned int& x, const unsigned int& y,const sizetPair& map_index, const size_t& index) {
	//this line looks a bit silly but all it does is bit shift the chunk down by whatever the cell bit offset is + 1 to get the three neighbour bits and not the state
	//then it masks it with 0b111 to get just the neighbours, then cast to a uint8_t because the max neighbour size is 7
	uint8_t value = cells[map_index.first] >> (map_index.second + 1) & (uint64_t(1) << (BIT_CELL_SIZE - 1)) - 1;
	

	//std::cout << columns << " " << 0 % 2 << " "  << getMapIndex({index + 1}).first << "\n";
	//if even go to the right for the missing neighbour value
	if (index % 2 == 0) {
		//check range of index and then wrap if larger than max width
		if (x + 1 == columns) {
			sizetPair new_mapIndex = getMapIndex({ y * columns });
			value += getState(new_mapIndex);
		}
		else {
			sizetPair new_mapIndex = getMapIndex({ index + 1 });
			value += getState(new_mapIndex);
		}
	}
	else { //if odd go to the left
		if (x - 1 < 0) {
			sizetPair new_mapIndex = getMapIndex({ y * columns + columns });
			value += getState(new_mapIndex);
		}
		else {
			sizetPair new_mapIndex = getMapIndex({ index - 1 });
			value += getState(new_mapIndex);
		}
	}

	return value;
} 