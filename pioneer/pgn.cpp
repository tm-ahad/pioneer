#include "pgn.h"

using namespace std;

Pgn::Pgn(const string& pgn)
{
    size_t move_start = pgn.find("\n\n");
    string moves_str = pgn.substr(0, move_start);

    size_t start = 0;
    size_t end = 0;

    Board board;

    for (char& c : moves_str) {
        if (c == '\n') {
            c = ' ';
        }
    }

    while ((end = moves_str.find(" ", start)) != string::npos)
    {
        string token = moves_str.substr(start, end - start);
        size_t dot = token.find(".");

        if (dot != string::npos)
        {
            token = token.substr(dot + 1);
        }

        if (token.empty())
        {
            start = end + 1;
            continue;
        }

        if (token == "1/2-1/2" || token == "1-0" || token == "0-1") {
            break;
        }

        Move move(board.sanToUci(token));

        board.makeMove(move);
        moves.push_back(move);

        start = end + 1;
    }
}

Move Pgn::getMove(size_t index) const
{
    return moves[index];
}

size_t Pgn::moveCount() const 
{
    return moves.size();
}
