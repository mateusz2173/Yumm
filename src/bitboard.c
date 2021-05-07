#include "../include/bitboard.h"

const U64 not_A_file = 0xfefefefefefefefe; // ~0x0101010101010101
const U64 not_H_file = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080

const int index64[64] = 
{
	0,  1, 48,  2, 57, 49, 28,  3,
	61, 58, 50, 42, 38, 29, 17,  4,
	62, 55, 59, 36, 53, 51, 43, 22,
	45, 39, 33, 30, 24, 18, 12,  5,
	63, 47, 56, 27, 60, 41, 37, 16,
	54, 35, 52, 21, 44, 32, 23, 11,
	46, 26, 40, 15, 34, 20, 31, 10,
	25, 14, 19,  9, 13,  8,  7,  6
};

const U64 debruijn64 = 0x03f79d71b4cb0a89;

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
	// 36 = 00100100
	pos->white_bishops = 36;
	// 66 = 01000010
	pos->white_knights = 66;
	// 129 = 100000001
	pos->white_rooks = 129;
	pos->white_king = (1 << 4);
	pos->white_queens = (1 << 3);

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

square bitscan_forward(U64 bb)
{
	return index64[((bb & -bb) * debruijn64) >> 58];
}

U64 north(U64 b) {return b << 8;}
U64 south(U64 b) {return b >> 8;}
U64 east (U64 b) {return (b << 1) & not_A_file;}
U64 west (U64 b) {return (b >> 1) & not_H_file;}
U64 north_east (U64 b) {return (b << 9) & not_A_file;}
U64 south_east (U64 b) {return (b >> 7) & not_A_file;}
U64 south_west (U64 b) {return (b >> 9) & not_H_file;}
U64 north_west (U64 b) {return (b << 7) & not_H_file;}

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
