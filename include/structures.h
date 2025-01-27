#pragma once
#include <vector>
#include <array>
#include <utility>
#include <cstdint>
#include <algorithm>
#include <barrier>
#include <unordered_set>



typedef std::pair<size_t,size_t> sizetPair;
typedef std::pair<uint8_t, uint8_t> u8Pair;
typedef std::vector<uint64_t> bitMap;

struct location {
	int x;
	int y;
	uint8_t state;

	location() : x(0), y(0), state(0) {};
	location(const int& nx, const int& ny, const uint8_t& nstate) : x(nx), y(ny), state(nstate) {};
};

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& pair) const {
		return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
	}
};

typedef std::unordered_set<std::pair<int, int>, pair_hash> set;

