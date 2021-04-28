#include "../include/piece.h"

const char piece_char[7] = {'0', 'P', 'N', 'B', 'R', 'Q', 'K'};
const int offset_count[6] = {0, 8, 4, 4, 8, 8};
const BYTE slide[6] = {0, 0, 1, 1, 1, 0};
const int offset[6][8] = 
{
	{   0,   0,  0,  0, 0,  0,  0,  0 },
	{ -21, -19,-12, -8, 8, 12, 19, 21 }, /* KNIGHT */
	{ -11,  -9,  9, 11, 0,  0,  0,  0 }, /* BISHOP */
	{ -10,  -1,  1, 10, 0,  0,  0,  0 }, /* ROOK */
	{ -11, -10, -9, -1, 1,  9, 10, 11 }, /* QUEEN */
	{ -11, -10, -9, -1, 1,  9, 10, 11 }  /* KING */
};

BYTE p_color(PIECE piece)
{
    return piece & (1 << 7);
}

BYTE p_castle(PIECE piece) 
{
    return piece & (1 << 4);
}

BYTE p_moved(PIECE piece)
{
    return piece & (1 << 3);
}

BYTE p_type(PIECE piece)
{
    return piece & 7; // 7 = (1 << 3) - 1
}

PIECE create_piece(BYTE color, BYTE castle, BYTE moved, BYTE type)
{
    return (color << 7) | (castle << 4) | (moved << 3) | type;
}

