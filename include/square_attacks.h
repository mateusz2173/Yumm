#pragma once

#include "bitboard.h"
#include "bitboard_attacks.h"

enum rays
{
	E, N, NE, NW, W, S, SW, SE
};

extern U64 rays_attacks[64][8];
extern U64 arr_king_attacks[64];

void init_rays_attacks();

U64 sq_rank_attacks(square sq);
U64 sq_file_attacks(square sq);
U64 sq_diagonal_attacks(square sq);
U64 sq_anti_diagonal_attacks(square sq);

U64 sq_obstructed_ray_attacks(U64 empty_squares, byte direction, square sq);

void precalc_king_attacks();

U64 sq_rook_attacks(U64 occ, square rook_sq);
U64 sq_bishop_attacks(U64 occ, square bishop_sq);

U64 xray_rook_attacks(U64 occ, U64 blockers, square rook_sq);
U64 xray_bishop_attacks(U64 occ, U64 blockers, square rook_sq);
