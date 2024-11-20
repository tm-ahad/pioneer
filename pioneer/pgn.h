#pragma once

#include <vector>
#include <string>
#include "board.h"

using namespace std;

class Pgn
{
private:
	vector<Move> moves;

public:
	Pgn(const string& pgn);
	Move getMove(size_t index) const;
	size_t moveCount() const;
};
