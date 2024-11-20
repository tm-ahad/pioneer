#pragma once

#include <stdexcept>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <cstdint>
#include "utils\split.h"
#include "piece.h"

using namespace std;

bool isLowerCase(char a);

class Square
{
public:
    Square(char _file, char _rank);

    char file;
    char rank;

    size_t toIndex() const;
    bool isNull() const;
    static Square null();
    static Square decode(char s);
    static Square fromString(string s);
    string toString() const;
};

class Move
{
private:
    char from;
    char to;

public:
    Move(const string& uci_mov);
    Move();

    int16_t encode() const;
    void decode(int16_t enc);
    char fromSquare() const;
    char toSquare() const;
    bool isNull() const;
    bool isCastle() const;
    string toUci() const;
    static Move null();
    static void fromSquares(Square to, Square from);
    bool cmp(const Move& other) const;

};

class Board
{
private:
    char board[64];
    bool sideToMove;

    Square findPiece(char piece, Square goal, Square ambgClarifier) const;

public:
    Board();
    char* encode() const;
    void decode(char* enc);
    void makeMove(const Move& move);
    static Board& fromFen(const string& fen);
    const char* representation() const;
    bool operator==(const Board& other) const;
    string sanToUci(string& uci) const;
    size_t indexFromFr(char file, char rank) const;
    void print() const;
};
