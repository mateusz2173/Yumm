#include "../include/move.h"

const U64 rank4 = 0x00000000FF000000;
const U64 rank5 = 0x000000FF00000000;

U64 rays_attacks[64][8];
U64 arr_king_attacks[64];

void init_rays_attacks()
{
	// directions' masks
	U64 nort = 0x0101010101010100; 
	U64 noea = 0x8040201008040200;	
	U64 est  = 0x00000000000000FE; 
	U64 sout = 0x0080808080808080;
	U64 wst =  0x7F00000000000000;
	U64 sowe = 0x0040201008040201;
	U64 nowe = 0x0102040810204000;
	U64 soea = 0x0002040810204080;

	for(int f = 0; f < 8; ++f)
	{
		U64 n = nort;
		U64 e = est;
		U64 ne = noea;
		U64 se = soea;
		for(int r = 0; r < 8; ++r)
		{
			rays_attacks[r * 8 + f][N] = n;	
			rays_attacks[r * 8 + f][E] = e;	
			rays_attacks[r * 8 + f][NE] = ne;	

			n = north(n);
			e = north(e);
			ne = north(ne);
		}
		for(int r = 7; r >= 0; --r, se = south(se))
		{
			rays_attacks[r * 8 + f][SE] = se;
		}

		nort = east(nort);
		est = east(est);
		noea = east(noea);
		soea = east(soea);
	}

	for(int f = 7; f >= 0; --f)
	{
		U64 s = sout;
		U64 w = wst;
		U64 sw = sowe;
		U64 nw = nowe;
		for(int r = 7; r >= 0; --r)
		{
			rays_attacks[r * 8 + f][S] = s;
			rays_attacks[r * 8 + f][W] = w;
			rays_attacks[r * 8 + f][SW] = sw;

			s = south(s);
			w = south(wst);
			sw = south(sw);
		}
		for(int r = 0; r < 8; ++r, nw = north(nw))
		{
			rays_attacks[r * 8 + f][NW] = nw;
		}
		sout = west(sout);
		wst = west(wst);
		sowe = west(sowe);
		nowe = west(nowe);
	}
}

U64 rank_attacks(square sq)
{
	return rays_attacks[sq][E] | rays_attacks[sq][W];
}

U64 file_attacks(square sq)
{
	return rays_attacks[sq][N] | rays_attacks[sq][S];
}

U64 diagonal_attacks(square sq)
{
	return rays_attacks[sq][NE] | rays_attacks[sq][SW];
}

U64 anti_diagonal_attacks(square sq)
{
	return rays_attacks[sq][NW] | rays_attacks[sq][SE];
}

U64 relative_ray_attacks(U64 empty_squares, byte direction, square sq)
{
	U64 occupied = ~empty_squares;
	U64 attacks = rays_attacks[sq][direction];
	U64 blocker = attacks & occupied;
	if( blocker )
	{
		if(direction <= NW) 
			sq = bitscan(blocker, 0);
		else
			sq = bitscan(blocker, 1);
		attacks ^= rays_attacks[sq][direction];
	}
	return attacks;
}

/* DIRECTION ATTACKS */

U64 south_attacks(U64 gen, U64 pro) 
{
	gen |= pro & (gen >>  8);
	pro &=       (pro >>  8);
	gen |= pro & (gen >> 16);
	pro &=       (pro >> 16);
	gen |= pro & (gen >> 32);
	return gen;
}

U64 north_attacks(U64 gen, U64 pro) 
{
	gen |= pro & (gen <<  8);
	pro &=       (pro <<  8);
	gen |= pro & (gen << 16);
	pro &=       (pro << 16);
	gen |= pro & (gen << 32);
	return gen;
}


U64 east_attacks(U64 gen, U64 pro) 
{
	pro &= not_A_file;
	gen |= pro & (gen << 1);
	pro &=       (pro << 1);
	gen |= pro & (gen << 2);
	pro &=       (pro << 2);
	gen |= pro & (gen << 4);
	return gen;
}

U64 north_east_attacks(U64 gen, U64 pro) 
{
	pro &= not_A_file;
	gen |= pro & (gen <<  9);
	pro &=       (pro <<  9);
	gen |= pro & (gen << 18);
	pro &=       (pro << 18);
	gen |= pro & (gen << 36);
	return gen;
}

U64 south_east_attacks(U64 gen, U64 pro) 
{
	pro &= not_A_file;
	gen |= pro & (gen >>  7);
	pro &=       (pro >>  7);
	gen |= pro & (gen >> 14);
	pro &=       (pro >> 14);
	gen |= pro & (gen >> 28);
	return gen;
}

U64 west_attacks(U64 gen, U64 pro) 
{
	pro &= not_H_file;
	gen |= pro & (gen >> 1);
	pro &=       (pro >> 1);
	gen |= pro & (gen >> 2);
	pro &=       (pro >> 2);
	gen |= pro & (gen >> 4);
	return gen;
}

U64 south_west_attacks(U64 gen, U64 pro)
{
	pro &= not_H_file;
	gen |= pro & (gen >>  9);
	pro &=       (pro >>  9);
	gen |= pro & (gen >> 18);
	pro &=       (pro >> 18);
	gen |= pro & (gen >> 36);
	return gen;
}

U64 north_west_attacks(U64 gen, U64 pro)
{
	pro &= not_H_file;
	gen |= pro & (gen <<  7);
	pro &=       (pro <<  7);
	gen |= pro & (gen << 14);
	pro &=       (pro << 14);
	gen |= pro & (gen << 28);
	return gen;
}

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

U64 rook_attacks(U64 rooks, U64 empty_squares)
{
	return north_attacks(rooks, empty_squares) 
		 | south_attacks(rooks, empty_squares)
		 | west_attacks(rooks, empty_squares)
		 | east_attacks(rooks, empty_squares);
}

U64 bishop_attacks(U64 bishops, U64 empty_squares)
{
	return north_east_attacks(bishops, empty_squares) 
		 | south_east_attacks(bishops, empty_squares)
		 | south_west_attacks(bishops, empty_squares)
		 | north_west_attacks(bishops, empty_squares);
}

U64 queen_attacks(U64 queens, U64 empty_squares)
{
	return rook_attacks(queens, empty_squares)
		 | bishop_attacks(queens, empty_squares);
}


void precalc_king_attacks(U64 *arr)
{
	U64 bb = 1;
	for(int i = 0; i < 64; ++i, bb <<= 1)
	{
		arr[i] = king_attacks(bb);
	}
}

