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

Book::Book() : variations(0) {}

bool Book::cmpMoveEntry(const MoveEntry& a, const MoveEntry& b)
{
    return a.count > b.count;
}

void Book::insertFromPgn(const Pgn& pgn)
{
    Board board;

    for (size_t i = 0; i < pgn.moveCount(); ++i)
    {
        Move move = pgn.getMove(i);
        insert(board, move);
        board.makeMove(move);
    }
}

void Book::resize_vector(unsigned char size)
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
    stream.write(reinterpret_cast<const char*>(&variations), sizeof(variations));
    for (const auto& pair : book)
    {
        char* board = pair.first.encode();
        stream.write(board, 32); // 32 bytes for board

        for (const auto& moveEntry : pair.second)
        {
            int16_t move = moveEntry.move.encode();
            stream.write(reinterpret_cast<const  char*>(&move), sizeof(int16_t)); // Encoded move
        }
    }
}

void Book::read_book(ifstream& stream)
{
    book.clear();
    stream.read(reinterpret_cast<char*>(&variations), sizeof(variations));

    char boardData[32];
    while (stream.read(boardData, 32))
    {
        Board board;
        board.decode(boardData);

        vector<MoveEntry> entries;
        int16_t moveBin = 0;
        while (stream.read(reinterpret_cast<char*>(&moveBin), sizeof(moveBin)))
        {
            Move move("a1a1");
            move.decode(moveBin);

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
