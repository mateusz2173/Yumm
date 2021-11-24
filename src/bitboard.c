#include "../include/bitboard.h"

const U64 not_A_file = 0xfefefefefefefefe; // ~0x0101010101010101
const U64 not_H_file = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080

const U64 rank4 = 0x00000000FF000000;
const U64 rank5 = 0x000000FF00000000;

// Some magic which makes bitscan efficient 
const int index64[64] = 
{
	0, 47,  1, 56, 48, 27,  2, 60,
	57, 49, 41, 37, 28, 16,  3, 61,
	54, 58, 35, 52, 50, 42, 21, 44,
	38, 32, 29, 23, 17, 11,  4, 62,
	46, 55, 26, 59, 40, 36, 15, 53,
	34, 51, 20, 43, 31, 22, 10, 45,
	25, 39, 14, 33, 19, 30,  9, 24,
	13, 18,  8, 12,  7,  6,  5, 63
};
const U64 debruijn64 = 0x03f79d71b4cb0a89;

const char SQ_NAMES[64][3] = 
{
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

const char PIECE_NAMES[2][6] =
{
    {'P', 'N', 'B', 'R', 'Q', 'K'},
    {'p', 'n', 'b', 'r', 'q', 'k'}
};

const wchar_t PIECE_CHARS[2][6] = 
{
    {0x265F, 0x265E, 0x265D, 0x265C, 0x265B, 0x265A},
    {0x2659, 0x2658, 0x2657, 0x2656, 0x2655, 0x2654}
};

U64 square_to_bitboard(square sq)
{
	return (U64) (1ULL << sq);
}

U64 pieces(position pos, byte color)
{
    U64 result = 0ULL;
    if(color == WHITE)
    {
        for(int i = 0; i < 6; ++i)
            result |= pos.pieces[WHITE][i];
    }
    else
    {
        for(int i = 0; i < 6; ++i)
            result |= pos.pieces[BLACK][i];
    }

    return result;
}

void update_empty_squares(position* pos)
{
    pos->empty_squares = ~(pieces(*pos, WHITE) | pieces(*pos, BLACK));
}

byte piece_on_square(position pos, square sq)
{
    for(int i = 0; i < 6; ++i)
    {
        if(_MASK_BIT(pos.pieces[WHITE][i] | pos.pieces[BLACK][i], sq))
            return i;
    }

    return EMPTY;
}

byte color_on_square(position pos, square sq)
{
    if(_MASK_BIT(pieces(pos, WHITE), sq))
        return WHITE;

    else if(_MASK_BIT(pieces(pos, BLACK), sq))
        return BLACK;

    return NONE;
}



// returns index of the most significant bit
square bitscan_reverse(U64 bb)
{
	bb |= bb >> 1; 
	bb |= bb >> 2;
	bb |= bb >> 4;
	bb |= bb >> 8;
	bb |= bb >> 16;
	bb |= bb >> 32;
	return index64[(bb * debruijn64) >> 58];
}


// Returns index of the least significant bit
// If reverse flag is true, returns most significant bit instead
square bitscan(U64 bb, byte reverse)
{
	U64 rMask = -(U64)reverse;
	bb &= -bb | rMask;
	return bitscan_reverse(bb);
}


// Some basic bb transformations
U64 north(U64 b) {return b << 8;}
U64 south(U64 b) {return b >> 8;}
U64 east (U64 b) {return (b << 1) & not_A_file;}
U64 west (U64 b) {return (b >> 1) & not_H_file;}
U64 north_east (U64 b) {return (b << 9) & not_A_file;}
U64 south_east (U64 b) {return (b >> 7) & not_A_file;}
U64 south_west (U64 b) {return (b >> 9) & not_H_file;}
U64 north_west (U64 b) {return (b << 7) & not_H_file;}

// Other


// for debug purposes
void print_board(U64 b)
{
	for(int r = 0; r < 8; ++r)
	{
		byte rank = b & 255;
		b = b >> 8;
		for(int i = 0; i < 8; ++i)
		{
			printf("%d  ", rank & 1);
			rank = rank >> 1;
		}
		printf("\n");
	}
}

void print_chess_board(U64 b)
{
	for(int r = 0; r < 8; ++r)
	{
		byte rank = (b >> (8 * (7 - r))) & 255;
		for(int i = 0; i < 8; ++i)
		{
			printf("%d  ", rank & 1);
			rank >>= 1;
		}
		printf("\n");
	}
}

#define _POS_DELIMETER ' '
void print_position(position pos)
{
    for(int r = 7; r >= 0; --r)
    {
        for(int f = 0; f <= 7; ++f)
        {
            square sq = r * 8 + f;
            byte color = color_on_square(pos, sq);

            if(color == NONE)
            {
                printf("%c  ", _POS_DELIMETER);
            }
            else 
            {
                byte type = piece_on_square(pos, sq);
                wprintf(L"%lc  ", PIECE_CHARS[color][type]);
            }
        }
        printf("\n");
    }
}
