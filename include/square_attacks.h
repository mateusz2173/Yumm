#pragma once

#include "bitboard.h"
#include "bitboard_attacks.h"

enum rays
{
	E, N, NE, NW, W, S, SW, SE
};

// precalculated lookup tables
extern U64 rays_attacks[64][8];
extern U64 arr_king_attacks[64];
extern U64 arr_pawn_attacks[2][64];
extern U64 arr_pawn_single_pushes[2][64];
extern U64 arr_pawn_double_pushes[2][64];
extern U64 arr_knight_attacks[64];

// initializes ray lookup tables
void init_rays_attacks();

// ray attacks
U64 sq_rank_attacks(square sq);
U64 sq_file_attacks(square sq);
U64 sq_diagonal_attacks(square sq);
U64 sq_anti_diagonal_attacks(square sq);

// returns attacked squares BB obstructed by blockers 
U64 sq_obstructed_ray_attacks(U64 empty_squares, byte direction, square sq);

// initializes non-ray lookup tables
void precalc_attacks();

U64 sq_rook_attacks(U64 occ, square rook_sq);
U64 sq_bishop_attacks(U64 occ, square bishop_sq);

// same as rook_attacks but ignores first blocker
U64 xray_rook_attacks(U64 occ, U64 blockers, square rook_sq);
U64 xray_bishop_attacks(U64 occ, U64 blockers, square rook_sq);

// masks all pieces which attacks given square
U64 attacks_to_sq(position pos, square sq, byte color);
