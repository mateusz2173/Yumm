#pragma once

#include "bitboard.h"

enum rays
{
	N, S, E, W, NW, NE, SW, SE
};

extern U64 rays_attacks[64][8];
extern U64 arr_king_attacks[64];

void init_rays_attacks();

/* PAWNS MOVES */
U64 white_single_push_targets(U64 white_pawns, U64 empty_squares);
U64 white_double_push_targets(U64 white_pawns, U64 empty_squares);
U64 black_single_push_targets(U64 black_pawns, U64 empty_squares);
U64 black_double_push_targets(U64 black_pawns, U64 empty_squares);

/* PAWNS ATTACKS */
U64 white_pawn_east_attacks(U64 white_pawns);
U64 white_pawn_west_attacks(U64 white_pawns);
U64 black_pawn_east_attacks(U64 white_pawns);
U64 black_pawn_west_attacks(U64 white_pawns);
U64 white_pawn_attacks(U64 white_pawns);
U64 black_pawn_attacks(U64 black_pawns);

/* OTHER PIECES ATTACKS */
U64 knight_attacks(U64 knights);
U64 king_attacks(U64 king);


/*  
U64 north_attacks(square sq, U64 empty_squares);
U64 north_west_attacks(square sq, U64 empty_squares);
U64 north_east_attacks(square sq, U64 empty_squares);
U64 south_attacks(square sq, U64 empty_squares);
U64 south_west_attacks(square sq, U64 empty_squares);
U64 south_east_attacks(square sq, U64 empty_squares);
*/

void precalc_king_attacks(U64* arr);
