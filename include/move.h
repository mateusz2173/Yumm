#pragma once

#include "bitboard.h"

enum rays
{
	E, N, NE, NW, W, S, SW, SE
};

extern U64 rays_attacks[64][8];
extern U64 arr_king_attacks[64];

void init_rays_attacks();

U64 rank_attacks(square sq);
U64 file_attacks(square sq);
U64 diagonal_attacks(square sq);
U64 anti_diagonal_attacks(square sq);

U64 relative_ray_attacks(U64 empty_squares, byte direction, square sq);

U64 east_attacks(U64 pieces, U64 empty_squares);

/* DIRECTION_ATTACKS */
U64 north_attacks(U64 gen, U64 pro);
U64 south_attacks(U64 gen, U64 pro);
U64 east_attacks(U64 gen, U64 pro);
U64 west_attacks(U64 gen, U64 pro);
U64 north_west_attacks(U64 gen, U64 pro);
U64 north_east_attacks(U64 gen, U64 pro);
U64 south_west_attacks(U64 gen, U64 pro);
U64 south_east_attacks(U64 gen, U64 pro);

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
U64 rook_attacks(U64 rooks, U64 empty_squares);
U64 bishop_attacks(U64 bishops, U64 empty_squares);
U64 queen_attacks(U64 queens, U64 empty_squares);


/*  
U64 north_attacks(square sq, U64 empty_squares);
U64 north_west_attacks(square sq, U64 empty_squares);
U64 north_east_attacks(square sq, U64 empty_squares);
U64 south_attacks(square sq, U64 empty_squares);
U64 south_west_attacks(square sq, U64 empty_squares);
U64 south_east_attacks(square sq, U64 empty_squares);
*/

void precalc_king_attacks(U64* arr);
