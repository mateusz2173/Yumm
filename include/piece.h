#pragma once

typedef unsigned char BYTE;
typedef BYTE PIECE;

enum piece_type
{
    empty, pawn, knight, bishop, rook, queen, king
};

extern const char piece_char[7];
extern const int offset_count[6];
extern const BYTE slide[6];
extern const int offset[6][8];

BYTE piece_color(PIECE piece);
BYTE piece_castle(PIECE piece);
BYTE piece_moved(PIECE piece);
BYTE piece_type(PIECE piece);

PIECE create_piece(BYTE color, BYTE castle, BYTE moved, BYTE type);
