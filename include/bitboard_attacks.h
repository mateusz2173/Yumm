#pragma once

#include "bitboard.h"

// Rays attacks with blocker excluded
U64 south_attacks(U64 gen, U64 pro);
U64 north_attacks(U64 gen, U64 pro);
U64 east_attacks(U64 gen, U64 pro);
U64 north_east_attacks(U64 gen, U64 pro);
U64 south_east_attacks(U64 gen, U64 pro);
U64 west_attacks(U64 gen, U64 pro);
U64 south_west_attacks(U64 gen, U64 pro);
U64 north_west_attacks(U64 gen, U64 pro);

// Rays attacks
U64 south_attacks(U64 gen, U64 pro);
U64 north_attacks(U64 gen, U64 pro);
U64 east_attacks(U64 gen, U64 pro);
U64 north_east_attacks(U64 gen, U64 pro);
U64 south_east_attacks(U64 gen, U64 pro);
U64 west_attacks(U64 gen, U64 pro);
U64 south_west_attacks(U64 gen, U64 pro);
U64 north_west_attacks(U64 gen, U64 pro);

// Pawn moves & attacks
U64 white_single_push_targets(U64 white_pawns, U64 empty_squares);
U64 white_double_push_targets(U64 white_pawns, U64 empty_squares);
U64 black_single_push_targets(U64 white_pawns, U64 empty_squares);
U64 black_double_push_targets(U64 white_pawns, U64 empty_squares);
U64 white_pawn_east_attacks(U64 white_pawns);
U64 white_pawn_west_attacks(U64 white_pawns);
U64 black_pawn_east_attacks(U64 white_pawns);
U64 black_pawn_west_attacks(U64 white_pawns);
U64 white_pawn_attacks(U64 white_pawns);
U64 black_pawn_attacks(U64 black_pawns);

// Other pieces attacks
U64 knight_attacks(U64 knights);
U64 king_attacks(U64 king);
U64 rook_attacks(U64 rooks, U64 empty_squares);
U64 bishop_attacks(U64 bishops, U64 empty_squares);
U64 queen_attacks(U64 queens, U64 empty_squares);

//general attacks
U64 bb_attacks_by(position pos, byte color);
