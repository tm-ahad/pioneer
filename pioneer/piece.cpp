#include "piece.h"

using namespace std;

char* encodePieces(const vector<Piece>& pieces)
{
	size_t size = pieces.size();
	char* encoded = new char[size];

	for (size_t i = 0; i < size; ++i)
	{
		encoded[i] = static_cast<char>(pieces[i]);
	}

	return encoded;
}

char encodePiece(Piece piece) 
{
	return static_cast<char>(piece);
}

Piece decodePiece(char piece)
{
	return static_cast<Piece>(piece);
}

char pieceToChar(Piece piece)
{
	switch (piece)
	{
	case WP: return 'P';
	case BP: return 'p';
	case WB: return 'B';
	case BB: return 'b';
	case WK: return 'K';
	case BK: return 'k';
	case WN: return 'N';
	case BN: return 'n';
	case WQ: return 'Q';
	case BQ: return 'q';
	case WR: return 'R';
	case BR: return 'r';
	case NN: return '-';
	}
}

char charToPiece(char c, bool sideToMove)
{
	switch (c) 
	{
		case 'P': case 'p': return sideToMove ? WP : BP;
		case 'N': case 'n': return sideToMove ? WN : BN;
		case 'B': case 'b': return sideToMove ? WB : BB;
		case 'R': case 'r': return sideToMove ? WR : BR;
		case 'Q': case 'q': return sideToMove ? WQ : BQ;
		case 'K': case 'k': return sideToMove ? WK : BK;
	}
}

