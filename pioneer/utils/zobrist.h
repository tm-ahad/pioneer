#pragma once
#include "../board.h"

typedef unsigned long long uint64_t;

class BoardHash 
{
public:
	uint64_t operator()(const Board& board) const;
};