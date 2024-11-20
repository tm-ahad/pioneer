#pragma once
#include <vector>

using namespace std;

enum Piece
{
	WP, // White pawn
	WN, // White knight
	WB, // White bishop
	WR, // White rook
	WQ, // White queen
	WK, // White king
	BP, // Black pawn
	BN, // Black night
	BB, // Black bishop
	BR, // Black rook
	BQ, // Black queen
	BK, // Black king,
	NN  // Empty
};

char charToPiece(char c, bool sideToMove);
char* encodePieces(const vector<Piece>& pieces);
char encodePiece(Piece piece);
char pieceToChar(Piece piece);
Piece decodePiece(char piece);

