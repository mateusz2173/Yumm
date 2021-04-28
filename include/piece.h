#ifndef PIECE_H
#define PIECE_H

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

BYTE p_color(PIECE piece);
BYTE p_castle(PIECE piece);
BYTE p_moved(PIECE piece);
BYTE p_type(PIECE piece);

PIECE create_piece(BYTE color, BYTE castle, BYTE moved, BYTE type);

#endif