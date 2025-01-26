#pragma once
#include <vector>
#include <array>
#include <utility>
#include <cstdint>
#include <algorithm>



typedef std::pair<size_t,size_t> sizetPair;
typedef std::pair<uint8_t, uint8_t> u8Pair;
typedef std::vector<uint64_t> bitMap;

struct location {
	const int x;
	const int y;
	const uint8_t state;
	location() : x(0), y(0), state(0) {};
	location(const int& nx, const int& ny, const uint8_t& nstate) : x(nx), y(ny), state(nstate) {};
};