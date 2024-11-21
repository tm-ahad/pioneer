#include <algorithm>
#include "book.h"

using namespace std;

MoveEntry::MoveEntry(Move _move, unsigned char _count)
    : move(_move), count(_count) {}

MoveEntry::MoveEntry() 
{
    move = Move::null();
    count = 0;
}

Book::Book(size_t variations, size_t moves) : variations(variations), moves(moves), pgns(0) {}

bool Book::cmpMoveEntry(const MoveEntry& a, const MoveEntry& b)
{
    return a.count > b.count;
}

void Book::insertFromPgn(const Pgn& pgn)
{
    Board board;
    pgns += 1;

    for (size_t i = 0; i < min(pgn.moveCount(), moves); ++i)
    {
        Move move = pgn.getMove(i);
        insert(board, move);
        board.makeMove(move);
    }
}

void Book::resize_vector(size_t size)
{
    for (auto& pair : book)
    {
        vector<MoveEntry>& entries = pair.second;
        sort(entries.begin(), entries.end(), Book::cmpMoveEntry);
        if (entries.size() > size)
        {
            entries.resize(size);
        }
    }
}

void Book::write_book(ostream& stream)
{
    variations = min(variations, pgns);
    stream.write(reinterpret_cast<const char*>(&variations), sizeof(variations));
    stream.write(reinterpret_cast<const char*>(&moves), sizeof(moves));

    for (const auto& pair : book)
    {
        char* board = pair.first.encode();
        stream.write(board, 32); // 32 bytes for board

        for (const auto& moveEntry : pair.second)
        {
            int16_t move = moveEntry.move.encode();
            stream.write(reinterpret_cast<const char*>(&move), sizeof(int16_t)); // Encoded move
        }
    }
}

void Book::read_book(ifstream& stream)
{
    book.clear();
    stream.read(reinterpret_cast<char*>(&variations), sizeof(variations));
    stream.read(reinterpret_cast<char*>(&moves), sizeof(moves));

    char boardData[32];
    while (stream.read(boardData, 32))
    {
        Board board;
        board.decode(boardData);

        vector<MoveEntry> entries;
        int16_t moveBin = 0;

        for (int i = 0; i < variations; i++)
        {
            stream.read(reinterpret_cast<char*>(&moveBin), sizeof(moveBin));
            Move move = Move::decode(moveBin);

            MoveEntry entry(move, 0);
            entries.push_back(entry);
        }

        book[board] = entries;
    }
}

void Book::insert(const Board& _board, const Move& move)
{
    vector<MoveEntry>& entries = book[_board];
    bool found = false;

    for (auto& entry : entries)
    {
        if (entry.move.encode() == move.encode())
        {
            entry.count += 1;
            found = true;
            break;
        }
    }

    if (!found)
    {
        MoveEntry newEntry(move, 1);
        entries.push_back(newEntry);
    }
}

void Book::setVariations(size_t _variations) 
{
    variations = _variations;
}

void Book::setMoveCount(size_t _moves)
{
    moves = _moves;
}

Move Book::getRankedMove(const Board& board, unsigned int rank)
{
    auto it = book.find(board);

    if (it != book.end()) 
    {
        if (it->second.size() > rank) 
        {
            return (it->second)[rank].move;
        }
        else 
        {
            return Move::null();
        }
    }
    else
    {
        return Move::null();
    }
}

Move Book::getRandMove(const Board& board)
{
    auto it = book.find(board);

    if (it != book.end()) {
        size_t rank = rng.generateByteNumber() % book[board].size();
        return (it->second)[rank].move;
    }
    else 
    {
        return Move::null();
    }
    
}

void Book::clear() 
{
    book.clear();

    pgns = 0;
    variations = 0;
    moves = 0;
}
