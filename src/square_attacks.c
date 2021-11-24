#include "../include/square_attacks.h"

U64 rays_attacks[64][8];
U64 arr_king_attacks[64];
U64 arr_pawn_attacks[2][64];
U64 arr_pawn_single_pushes[2][64];
U64 arr_pawn_double_pushes[2][64];
U64 arr_knight_attacks[64];

// fills array rays_attacks with data
void init_rays_attacks()
{
	// directions' masks
	U64 nort = 0x0101010101010100; 
	U64 noea = 0x8040201008040200;	
	U64 est  = 0x00000000000000FE; 
	U64 sout = 0x0080808080808080;
	U64 wst  = 0x7F00000000000000;
	U64 sowe = 0x0040201008040201;
	U64 nowe = 0x0102040810204000;
	U64 soea = 0x0002040810204080;

    // north attacks
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

    // south attacks
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
			w = south(w);
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

// returns all attacks in given direction from given square limited by blockes 
U64 sq_obstructed_ray_attacks(U64 empty_squares, byte direction, square sq)
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

U64 sq_rook_attacks(U64 occ, square rook_sq)
{
    U64 empty_squares = ~occ;
    return sq_obstructed_ray_attacks(empty_squares, N, rook_sq)
        |  sq_obstructed_ray_attacks(empty_squares, S, rook_sq)
        |  sq_obstructed_ray_attacks(empty_squares, E, rook_sq)
        |  sq_obstructed_ray_attacks(empty_squares, W, rook_sq);
}

U64 sq_bishop_attacks(U64 occ, square bishop_sq)
{
    U64 empty_squares = ~occ;
    return sq_obstructed_ray_attacks(empty_squares, NE, bishop_sq)
        |  sq_obstructed_ray_attacks(empty_squares, NW, bishop_sq)
        |  sq_obstructed_ray_attacks(empty_squares, SE, bishop_sq)
        |  sq_obstructed_ray_attacks(empty_squares, SW, bishop_sq);
}

void precalc_attacks()
{
	U64 bb = 1;
	for(int i = 0; i < 64; ++i, bb <<= 1)
	{
        U64 pushed_north = north(bb);
        U64 pushed_south = south(bb);
		arr_king_attacks[i] = king_attacks(bb);
        arr_pawn_attacks[WHITE][i] = white_pawn_attacks(bb);
        arr_pawn_attacks[BLACK][i] = black_pawn_attacks(bb);
        arr_pawn_single_pushes[WHITE][i] = pushed_north; 
        arr_pawn_single_pushes[BLACK][i] = pushed_south;
        arr_pawn_double_pushes[WHITE][i] = north(pushed_north) & rank4;
        arr_pawn_double_pushes[BLACK][i] = south(pushed_south) & rank5;
        arr_knight_attacks[i] = knight_attacks(bb);
	}
}


/* XRAY ATTACKS */

U64 xray_rook_attacks(U64 occ, U64 blockers, square rook_square)
{
    U64 attacks = sq_rook_attacks(occ, rook_square);
    blockers &= attacks;
    return attacks ^ sq_rook_attacks(occ ^ blockers, rook_square);
}

U64 xray_bishop_attacks(U64 occ, U64 blockers, square bishop_square)
{
    U64 attacks = sq_bishop_attacks(occ, bishop_square);
    blockers &= attacks;
    return attacks ^ sq_bishop_attacks(occ ^ blockers, bishop_square);
}

U64 attacks_to_sq(position pos, square sq, byte color)
{
    U64 occ = pieces(pos, WHITE) | pieces(pos, BLACK);
    U64* pieces = pos.pieces[color];
    U64 enpass_attack;
    if(sq == pos.enpass_square)
    {
        //TODO EN-PASSANT  
    }

    return (arr_pawn_attacks[1 - color][sq] & pieces[PAWN])
        |  (arr_knight_attacks[sq] & pieces[KNIGHT])
        |  (arr_king_attacks[sq] & pieces[KING])
        |  (sq_rook_attacks(occ, sq) & (pieces[ROOK] | pieces[QUEEN]))
        |  (sq_bishop_attacks(occ, sq) & (pieces[BISHOP] | pieces[QUEEN]));
}
