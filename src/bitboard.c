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
    {'P', 'B', 'N', 'R', 'Q', 'K'},
    {'p', 'b', 'n', 'r', 'q', 'k'}
};

U64 square_to_bitboard(square sq)
{
	return (U64) (1ULL << sq);
}

position* create_starting_position()
{
	position* pos = (position*) malloc(sizeof(position));

	if(!pos)
	{
		fprintf(stderr, "Couldn't allocate memory.");
		return NULL;
	}

	pos->turn = white;
	pos->castle_perm = CASTLE_WK | CASTLE_WQ | CASTLE_BK | CASTLE_BQ;
	pos->enpass_square = -1;
	pos->draw_move_counter = 0;
	
	// 255 = 11111111
	pos->white_pawns = (255 << 8);
	pos->white_bishops = (1 << c1) | (1 << f1);
	pos->white_knights = (1 << b1) | (1 << g1);
	pos->white_rooks = (1 << a1) | (1 << h1);
	pos->white_king = (1 << e1);
	pos->white_queens = (1 << d1);

	pos->black_pawns = pos->white_pawns << (8 * 5);
	pos->black_bishops = pos->white_bishops << (8 * 7);
	pos->black_knights = pos->white_knights << (8 * 7);
	pos->black_rooks = pos->white_rooks << (8 * 7);
	pos->black_queens = pos->white_queens << (8 * 7);
	pos->black_king = pos->white_king << (8 * 7);

	U64 double_rank = (1 << 16) - 1;
	pos->empty_squares = ~((double_rank << (8 * 6)) | double_rank);

	return pos;
}

U64 pieces(position pos, byte color)
{
    if(color == white)
    {
        return pos.white_king    | pos.white_pawns
            |  pos.white_rooks   | pos.white_queens 
            |  pos.white_bishops | pos.white_knights;
    }
    else
    {
        return pos.black_king    | pos.black_pawns
            |  pos.black_rooks   | pos.black_queens 
            |  pos.black_bishops | pos.black_knights;
    }
}

void update_empty_squares(position* pos)
{
    pos->empty_squares = ~(pieces(*pos, white) | pieces(*pos, black));
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
