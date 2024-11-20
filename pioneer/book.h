#pragma once

#include "utils/zobrist.h"
#include "utils/rng.h"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include "pgn.h"

using namespace std;

struct MoveEntry
{
    unsigned char count;
    Move move;

    MoveEntry(Move _move, unsigned char _count);
    MoveEntry();
};

class Book
{
private:
    static bool cmpMoveEntry(const MoveEntry& a, const MoveEntry& b);
    unordered_map<Board, vector<MoveEntry>, BoardHash> book;
    RandomNumberGenerator rng;
    unsigned char variations;

public:
    Book();

    void insertFromPgn(const Pgn& pgn);
    void resize_vector(unsigned char size);
    void write_book(ostream& stream);
    void read_book(ifstream& stream);
    void insert(const Board& board, const Move& move);
    Move getRankedMove(const Board& board, unsigned int rank);
    Move getRandMove(const Board& board);
};
