#include "../include/bitboard_attacks.h"

/* BB RAYS ATTACKS OCCLUDED */

U64 south_attacks_occl(U64 gen, U64 pro) 
{
	gen |= pro & (gen >>  8);
	pro &=       (pro >>  8);
	gen |= pro & (gen >> 16);
	pro &=       (pro >> 16);
	gen |= pro & (gen >> 32);
	return gen;
}

U64 north_attacks_occl(U64 gen, U64 pro) 
{
	gen |= pro & (gen <<  8);
	pro &=       (pro <<  8);
	gen |= pro & (gen << 16);
	pro &=       (pro << 16);
	gen |= pro & (gen << 32);
	return gen;
}


U64 east_attacks_occl(U64 gen, U64 pro) 
{
	pro &= not_A_file;
	gen |= pro & (gen << 1);
	pro &=       (pro << 1);
	gen |= pro & (gen << 2);
	pro &=       (pro << 2);
	gen |= pro & (gen << 4);
	return gen;
}

U64 north_east_attacks_occl(U64 gen, U64 pro) 
{
	pro &= not_A_file;
	gen |= pro & (gen <<  9);
	pro &=       (pro <<  9);
	gen |= pro & (gen << 18);
	pro &=       (pro << 18);
	gen |= pro & (gen << 36);
	return gen;
}

U64 south_east_attacks_occl(U64 gen, U64 pro) 
{
	pro &= not_A_file;
	gen |= pro & (gen >>  7);
	pro &=       (pro >>  7);
	gen |= pro & (gen >> 14);
	pro &=       (pro >> 14);
	gen |= pro & (gen >> 28);
	return gen;
}

U64 west_attacks_occl(U64 gen, U64 pro) 
{
	pro &= not_H_file;
	gen |= pro & (gen >> 1);
	pro &=       (pro >> 1);
	gen |= pro & (gen >> 2);
	pro &=       (pro >> 2);
	gen |= pro & (gen >> 4);
	return gen;
}

U64 south_west_attacks_occl(U64 gen, U64 pro)
{
	pro &= not_H_file;
	gen |= pro & (gen >>  9);
	pro &=       (pro >>  9);
	gen |= pro & (gen >> 18);
	pro &=       (pro >> 18);
	gen |= pro & (gen >> 36);
	return gen;
}

U64 north_west_attacks_occl(U64 gen, U64 pro)
{
	pro &= not_H_file;
	gen |= pro & (gen <<  7);
	pro &=       (pro <<  7);
	gen |= pro & (gen << 14);
	pro &=       (pro << 14);
	gen |= pro & (gen << 28);
	return gen;
}

/* BB ATTACKS */

U64 south_attacks(U64 gen, U64 pro)
{
    return south(south_attacks_occl(gen, pro));
}

U64 north_attacks(U64 gen, U64 pro)
{
    return north(north_attacks_occl(gen, pro));
}

U64 east_attacks(U64 gen, U64 pro)
{
    return east(east_attacks_occl(gen, pro));
}

U64 west_attacks(U64 gen, U64 pro)
{
    return west(west_attacks_occl(gen, pro));
}

U64 north_east_attacks(U64 gen, U64 pro)
{
    return north_east(north_east_attacks_occl(gen, pro));
}

U64 south_east_attacks(U64 gen, U64 pro)
{
    return south_east(south_east_attacks_occl(gen, pro));
}

U64 south_west_attacks(U64 gen, U64 pro)
{
    return south_west(south_west_attacks_occl(gen, pro));
}

U64 north_west_attacks(U64 gen, U64 pro)
{
    return north_west(north_west_attacks_occl(gen, pro));
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

// General attacks
U64 bb_attacks_by(position pos, byte color)
{
    U64 bb_RQ = pos.pieces[color][ROOK] | pos.pieces[color][QUEEN];
    U64 bb_BQ = pos.pieces[color][BISHOP] | pos.pieces[color][QUEEN];

    U64 occ = pieces(pos, WHITE) | pieces(pos, BLACK);

    return rook_attacks(bb_RQ, ~occ) 
        |  bishop_attacks(bb_BQ, ~occ)
        |  knight_attacks(pos.pieces[color][KNIGHT])
        |  white_pawn_attacks(pos.pieces[color][PAWN]);
}


