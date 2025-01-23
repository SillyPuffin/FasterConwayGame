#pragma once

#include <bitset>
#include <raylib.h>
#include <iostream>

//my code
#include "game.h"

void Game::DrawBackground() {
	ClearBackground(Color{ 50, 50, 50, 255 });
	Color Black = { 0,0,0,255 };

	for (int column = 0; column < columns; column++) {
		int x = column * cellSize;
		DrawLine(x, 0, x, columns * cellSize, Black);
	}

	for (int row = 0; row < rows; row++) {
		int y = row * cellSize;
		DrawLine(0,y, cellSize*rows,y, Black);
	}

}

void Game::DrawCells() {
	for (unsigned int x = 0; x < columns; x++) {
		for (unsigned int y = 0; y < rows; y++) {
			uint64_t state = getState(x, y);

			if (state == 1) {
				DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, Color{ 0,255,0,255 });
			}
		}
	}
}

sizetPair Game::getIndex(int x, int y) {
	size_t bit_index = (y * rows + x) * BIT_CELL_SIZE;
	size_t chunk_idx = bit_index / 64;
	size_t chunk_offset = bit_index % 64;

	return sizetPair{ chunk_idx, chunk_offset };
}

void Game::setCell(int x, int y) {
	sizetPair mapIndex = getIndex(x, y);

	//clearing the bits at the index position
	cells[mapIndex.first] &= ~(uint64_t((1 << BIT_CELL_SIZE)-1) << mapIndex.second);
	//setting the state of the bit to true
	cells[mapIndex.first] |= (uint64_t(1) << mapIndex.second);


	std::cout << std::bitset<64>(cells[mapIndex.first]) << "\n";
}

uint64_t Game::getState(int x, int y) {
	sizetPair mapIndex = getIndex(x, y);
	uint64_t state = uint64_t(1) & (cells[mapIndex.first] >> mapIndex.second);

	return state;
}