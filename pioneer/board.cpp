#include "board.h"

constexpr auto NULL_FILE_RANK = 0x30;

using namespace std;

bool isLowerCase(char a)
{
    return (a >= 'a') && (a <= 'z');
}

Square::Square(char _file, char _rank)
{
    file = _file;
    rank = _rank;
}

size_t Square::toIndex() const 
{
    return (rank * 8) + file;
}

Square Square::decode(char s) 
{
    Square sqr(s & 0b111, s >> 3);
    return sqr;
}

bool Square::isNull() const  
{
    return (file == NULL_FILE_RANK) && (rank == NULL_FILE_RANK);
}

Square Square::null()
{
    Square square(NULL_FILE_RANK, NULL_FILE_RANK);
    return square;
}

Square Square::fromString(string s)
{
    Square sqr = Square::null();

    switch (s.length()) 
    {
    case 2:
    {
        sqr.file = s[0] - 'a';
        sqr.rank = 8 - (s[1] - '0');
        break;
    }
    case 1:
    {
        if (('0' >= s[0]) && (s[0] <= '9')) 
        {
            sqr.rank = 8 - (s[1] - '0');
            sqr.file = NULL_FILE_RANK;
        }
        else 
        {
            sqr.file = s[0] - 'a';
            sqr.rank = NULL_FILE_RANK;
        }
        break;
    }
    }

    return sqr;
}

string Square::toString() const
{
    return string() + (char)(file + 'a') + (char)((8 - rank) + '0');
}

bool Move::isCastle() const 
{
    Move e1g1("e1g1");
    Move e1c1("e1c1");
    Move e8g8("e8g8");
    Move e8c8("e8c8");

    return this->cmp(e1g1)
        || this->cmp(e1c1)
        || this->cmp(e8g8)
        || this->cmp(e8c8);
}

Move::Move(const string& uci_mov)
{
    if (uci_mov.length() != 4)
    {
        throw invalid_argument("Invalid UCI move format. Expected format:: 'e2e4'.");
    }

    string from_square = uci_mov.substr(0, 2);
    string to_square = uci_mov.substr(2, 2);

    char file1 = from_square[0] - 'a';
    char file2 = to_square[0] - 'a';

    if (!isLowerCase(from_square[0]) || !isLowerCase(to_square[0]))
    {
        throw invalid_argument("Invalid UCI move. File must be lowercase characters. Example:: 'e2e4'.");
    }

    char rank1 = 8 - (from_square[1] - '0');
    char rank2 = 8 - (to_square[1] - '0');

    from = (rank1 << 3) + file1;
    to = (rank2 << 3) + file2;
}

void Move::fromSquares(Square to, Square from) 
{
    from = from;
    to = to;
}

int16_t Move::encode() const
{
    return (from << 3) | to;
}

void Move::decode(int16_t enc)
{
    from = enc >> 3;
    to = enc & 0xFF;
}

char Move::fromSquare() const
{
    return from;
}

char Move::toSquare() const
{
    return to;
}

bool Move::isNull() const
{
    return to == from;
}

Move::Move() 
{
    from = NULL_FILE_RANK;
    to = NULL_FILE_RANK;
}

Move Move::null() 
{
    Move null("a1a1");
    return null;
}

string Move::toUci() const
{
    char from_file = (from & 0b111) + 'a';
    char from_rank = '8' - (from >> 3);

    char to_file = (to & 0b111) + 'a';
    char to_rank = '8' - (to >> 3);

    return string() + from_file + from_rank + to_file + to_rank;
}

bool Move::cmp(const Move& other) const
{
    return (from == other.from) && (to == other.to);
}

Board::Board()
{
    Piece initPieces[64] =
    {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, BP, BP, BP, BP, BP, BP,
        NN, NN, NN, NN, NN, NN, NN, NN,
        NN, NN, NN, NN, NN, NN, NN, NN,
        NN, NN, NN, NN, NN, NN, NN, NN,
        NN, NN, NN, NN, NN, NN, NN, NN,
        WP, WP, WP, WP, WP, WP, WP, WP,
        WR, WN, WB, WQ, WK, WB, WN, WR
    };

    for (int i = 0; i < 64; i++)
    {
        board[i] = initPieces[i];
    }

    sideToMove = true;
}

char* Board::encode() const
{
    static char enc[32] = { 0 };

    for (char i = 0; i < 32; i++)
    {
        enc[i] = (board[i * 2] << 4) | board[(i * 2) + 1];
    }

    return enc;
}

void Board::decode(char* enc)
{
    for (char i = 0; i < 32; i++)
    {
        board[i * 2] = (enc[i] >> 4) & 0b1111;
        board[(i * 2) + 1] = enc[i] & 0b1111;
    }
}

const char* Board::representation() const
{
    return reinterpret_cast<const char*>(board);
}

void Board::makeMove(const Move& move)
{
    if (move.isCastle()) 
    {
        Square toRook = Square::decode(move.toSquare());
        toRook.file -= 1;

        Square fromRook = Square::decode(move.toSquare());
        fromRook.file += 1;

        board[toRook.toIndex()] = board[fromRook.toIndex()];
        board[fromRook.toIndex()] = NN;
    }

    board[move.toSquare()] = board[move.fromSquare()];
    board[move.fromSquare()] = NN;
    sideToMove = !sideToMove;
}

void Board::print() const
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++) 
        {
            cout << pieceToChar(decodePiece(board[(i * 8) + j])) << " ";
        }

        cout << endl;
    }
}

Board& Board::fromFen(const string& fen)
{
    static Board board;
    vector<string> parts = split(fen, " ");

    string layout = parts[0];
    vector<Piece> pieces(64, NN);

    int index = 0;
    for (char c : layout)
    {
        if (isdigit(c))
        {
            index += c - '0';
        }
        else
        {
            switch (c)
            {
            case 'p': pieces[index] = BP; break;
            case 'r': pieces[index] = BR; break;
            case 'n': pieces[index] = BN; break;
            case 'b': pieces[index] = BB; break;
            case 'q': pieces[index] = BQ; break;
            case 'k': pieces[index] = BK; break;
            case 'P': pieces[index] = WP; break;
            case 'R': pieces[index] = WR; break;
            case 'N': pieces[index] = WN; break;
            case 'B': pieces[index] = WB; break;
            case 'Q': pieces[index] = WQ; break;
            case 'K': pieces[index] = WK; break;
            case '/': continue;
            default: throw invalid_argument("Invalid fen.");
            }
            index++;
        }
    }

    for (int i = 0; i < 64; i++) {
        board.board[i] = pieces[i];
    }

    board.sideToMove = parts[1] == "w" ? true : false;
    return board;
}

bool Board::operator==(const Board& other) const 
{
    const char* rep1 = representation();
    const char* rep2 = other.representation();

    return strncmp(rep1, rep2, 64) == 0;
}

size_t Board::indexFromFr(char file, char rank) const 
{
    return (rank << 3) + file;
}

string Board::sanToUci(string& san) const
{
    if (san.back() == '#' || san.back() == '+')
    {
        san = san.substr(0, san.length() - 1);
    }

    if (san == "O-O") 
    {
        return sideToMove ? "e1g1" : "e8g8";
    }
    else if (san == "O-O-O") 
    {
        return sideToMove ? "e1c1" : "e8c8";
    }

    if (san.length() < 2) 
    {
        throw invalid_argument("Invalid SAN move");
    }

    string promotion = "";
    string move = san;

    size_t promotion_pos = san.find('=');
    if (promotion_pos != string::npos)
    {
        promotion = string(1, tolower(san[promotion_pos + 1]));
        move = san.substr(0, promotion_pos);
    }

    if (move.length() == 2 && islower(move[0]) && isdigit(move[1]))
    {
        char file = move[0] - 'a';
        char rank = 8 - (move[1] - '0');
        char o_rank;

        if ((board[indexFromFr(file, (o_rank = rank - 1))] == BP && !sideToMove)
            || (board[indexFromFr(file, (o_rank = rank + 1))] == WP && sideToMove)
            || (rank == 4 && board[indexFromFr(file, (o_rank = rank + 2))] == WP)
            || (rank == 3 && board[indexFromFr(file, (o_rank = rank - 2))] == BP))
        {
            char fileLetter = file + 'a';
            char rankNumber = (8 - o_rank) + '0';

            return string() + fileLetter + rankNumber + san;
        }
        else 
        {
            throw invalid_argument("Invalid move.");
        }
    }

    Square ambiguity = Square::null();
    Square goal = Square::null();
    size_t xI = 0;
    char piece;

    if ((xI = san.find('x')) != string::npos) 
    {
        bool isPawn = islower(san[0]);

        if (isPawn) 
        {
            ambiguity = Square::fromString(san.substr(0, xI));
            goal = Square::fromString(san.substr(xI + 1));
            piece = 'P';
        }
        else 
        {
            ambiguity = Square::fromString(san.substr(1, xI-1));
            goal = Square::fromString(san.substr(xI + 1));
            piece = san[0];
        }
    }
    else 
    {
        if (islower(san[0])) 
        {
            ambiguity = Square::fromString(san.substr(0, 1));
            piece = 'P';
        }
        else 
        {
            ambiguity = Square::fromString(san.substr(1, san.length() - 3));
            piece = san[0];
        }
        
        goal = Square::fromString(san.substr(san.length() - 2, 2));
    }

    return findPiece(piece, goal, ambiguity).toString() + goal.toString();
}

Square Board::findPiece(char piece, Square goal, Square ambgClarifier) const
{
    vector<Square> candidates;

    switch (piece)
    {
    case 'N': case 'n':
    {
        int knightOffsets[8][2] =
        {
            {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
            {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
        };

        for (int i = 0; i < 8; i++)
        {
            Square newSQ(goal.file + knightOffsets[i][0], goal.rank + knightOffsets[i][1]);

            if (ambgClarifier.file != NULL_FILE_RANK && newSQ.file != ambgClarifier.file)
            {
                continue;
            }

            if (ambgClarifier.rank != NULL_FILE_RANK && newSQ.rank != ambgClarifier.rank)
            {
                continue;
            }

            if (board[newSQ.toIndex()] == charToPiece(piece, sideToMove))
            {
                candidates.push_back(newSQ);
            }
        }
        break;
    }

    case 'B': case 'b':
    {
        int bishopOffsets[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

        for (int i = 0; i < 4; i++)
        {
            Square newSQ(goal.file + bishopOffsets[i][0], goal.rank + bishopOffsets[i][1]);

            while (newSQ.file >= 0 && newSQ.file < 8 && newSQ.rank >= 0 && newSQ.rank < 8)
            {
                if (ambgClarifier.file != NULL_FILE_RANK && newSQ.file != ambgClarifier.file)
                {
                    break;
                }

                if (ambgClarifier.rank != NULL_FILE_RANK && newSQ.rank != ambgClarifier.rank)
                {
                    break;
                }

                if (board[newSQ.toIndex()] == charToPiece(piece, sideToMove))
                {
                    candidates.push_back(newSQ);
                    break;
                }

                newSQ.file += bishopOffsets[i][0];
                newSQ.rank += bishopOffsets[i][1];
            }
        }
        break;
    }

    case 'R': case 'r': // Rook
    {
        int rookOffsets[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

        for (int i = 0; i < 4; i++)
        {
            Square newSQ(goal.file + rookOffsets[i][0], goal.rank + rookOffsets[i][1]);

            while (newSQ.file >= 0 && newSQ.file < 8 && newSQ.rank >= 0 && newSQ.rank < 8)
            {
                if (ambgClarifier.file != NULL_FILE_RANK && newSQ.file != ambgClarifier.file)
                {
                    break;
                }

                if (ambgClarifier.rank != NULL_FILE_RANK && newSQ.rank != ambgClarifier.rank)
                {
                    break;
                }

                if (board[newSQ.toIndex()] == charToPiece(piece, sideToMove))
                {
                    candidates.push_back(newSQ);
                    break;
                }
                  
                newSQ.file += rookOffsets[i][0];
                newSQ.rank += rookOffsets[i][1];
            }
        }
        break;
    }

    case 'Q': case 'q':
    {
        int queenOffsets[8][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

        for (int i = 0; i < 8; i++)
        {
            Square newSQ(goal.file + queenOffsets[i][0], goal.rank + queenOffsets[i][1]);

            while (newSQ.file >= 0 && newSQ.file < 8 && newSQ.rank >= 0 && newSQ.rank < 8)
            {
                if (ambgClarifier.file != NULL_FILE_RANK && newSQ.file != ambgClarifier.file)
                {
                    break;
                }

                if (ambgClarifier.rank != NULL_FILE_RANK && newSQ.rank != ambgClarifier.rank)
                {
                    break;
                }

                if (board[newSQ.toIndex()] == charToPiece(piece, sideToMove))
                {
                    candidates.push_back(newSQ);
                    break;
                }

                newSQ.file += queenOffsets[i][0];
                newSQ.rank += queenOffsets[i][1];
            }
        }
        break;
    }

    case 'K': case 'k':
    {
        int kingOffsets[8][2] =
        {
            {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
            {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        };

        for (int i = 0; i < 8; i++)
        {
            Square newSQ(goal.file + kingOffsets[i][0], goal.rank + kingOffsets[i][1]);

            if (ambgClarifier.file != NULL_FILE_RANK && newSQ.file != ambgClarifier.file)
            {
                continue;
            }

            if (ambgClarifier.rank != NULL_FILE_RANK && newSQ.rank != ambgClarifier.rank)
            {
                continue;
            }

            if (board[newSQ.toIndex()] == charToPiece(piece, sideToMove))
            {
                candidates.push_back(newSQ);
            }
        }
        break;
    }

    case 'P': case 'p':
    {
        if (ambgClarifier.file != NULL_FILE_RANK) 
        {
            int direction = sideToMove ? 1 : -1;
            Square oneStep(ambgClarifier.file, goal.rank + direction);

            if (board[oneStep.toIndex()] == charToPiece(piece, sideToMove))
            {
                candidates.push_back(oneStep);
            }
        }
        else 
        {
            for (int i = 1; i < 3; i++) 
            {
                int direction = sideToMove ? -i : i;
                Square step(ambgClarifier.file, goal.rank + direction);

                if (board[step.toIndex()] == charToPiece(piece, sideToMove))
                {
                    candidates.push_back(step);
                }
            }
        }

        break;
    }

    default:
        throw invalid_argument("Unknown piece type");
    }

    if (candidates.size() == 0) 
    {
        throw invalid_argument("invalid move.");
    }

    return candidates[0];
}
