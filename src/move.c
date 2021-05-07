#include "../include/move.h"

const U64 rank4 = 0x00000000FF000000;
const U64 rank5 = 0x000000FF00000000;

U64 rays_attacks[64][8];

void init_rays_attacks()
{
	// directions's masks
	U64 nort = 0x0101010101010100; 
	U64 noea = 0x8040201008040200;	
	U64 est  = 0x00000000000000FE; 

	for (square sq=0; sq < 64; sq++, nort <<= 1)
	   rays_attacks[sq][N] = nort;

	for (int f=0; f < 8; f++, noea = east(noea))
	{
		U64 ne = noea;
		for (int r8 = 0; r8 < 8*8; r8 += 8, ne <<= 8)
			rays_attacks[r8+f][NE] = ne;
	}

	for(int f = 0; f < 8; ++f, est = east(est))
	{
		U64 e = est;
		for(int r = 0; r < 8; ++r, e = north(e))
		{
			rays_attacks[r * 8 + f][E] = e;	
		}
	}

}

U64 arr_king_attacks[64];

/* PAWNS MOVES */

U64 white_single_push_targets(U64 white_pawns, U64 empty_squares)
{
	return north(white_pawns) & empty_squares;
}

U64 white_double_push_targets(U64 white_pawns, U64 empty_squares)
{
	U64 single_pushes = white_single_push_targets(white_pawns, empty_squares);
	return north(single_pushes) & empty_squares & rank4;
}

U64 black_single_push_targets(U64 black_pawns, U64 empty_squares)
{
	return south(black_pawns) & empty_squares;
}

U64 black_double_push_targets(U64 black_pawns, U64 empty_squares)
{
	U64 single_pushes = black_single_push_targets(black_pawns, empty_squares);
	return south(single_pushes) & empty_squares & rank5;
}

/* PAWNS ATTACKS */

U64 white_pawn_east_attacks(U64 white_pawns)
{
	return north_east(white_pawns);
}

U64 white_pawn_west_attacks(U64 white_pawns)
{
	return north_west(white_pawns);
}

U64 black_pawn_east_attacks(U64 black_pawns)
{
	return south_east(black_pawns);
}

U64 black_pawn_west_attacks(U64 black_pawns)
{
	return south_west(black_pawns);
}

U64 white_pawn_attacks(U64 white_pawns)
{
	return white_pawn_east_attacks(white_pawns) |
		   white_pawn_west_attacks(white_pawns);
}

U64 black_pawn_attacks(U64 black_pawns)
{
	return black_pawn_east_attacks(black_pawns) |
		   black_pawn_west_attacks(black_pawns);
}

/* OTHER ATTACKS */

U64 knight_attacks(U64 knights) 
{
	U64 e, w, attacks;
	e     = east(knights);
	w     = west(knights);
	attacks  = (e|w) << 16;
	attacks |= (e|w) >> 16;
	e     = east(e);
	w     = west(w);
	attacks |= (e|w) <<  8;
	attacks |= (e|w) >>  8;
	return attacks;
}

U64 king_attacks(U64 king)
{
	U64 attacks = east(king) | west(king);
	king |= attacks;
	attacks |= north(king) | south(king);
	return attacks;
}

void precalc_king_attacks(U64 *arr)
{
	U64 bb = 1;
	for(int i = 0; i < 64; ++i, bb <<= 1)
	{
			arr[i] = king_attacks(bb);
	}
}

