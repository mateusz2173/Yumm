#include "../include/game_utility.h"

U64 find_absolute_pins(position pos)
{
    U64 pinned = 0ULL;
    U64 opRQ, opBQ;
    square king_square;
    if(pos.turn == white)
    {
        opRQ = pos.black_rooks | pos.black_queens;
        opBQ = pos.black_bishops | pos.black_queens;
        king_square = bitscan(pos.white_king, 0);
    }
    else
    {
        opRQ = pos.white_rooks | pos.white_queens;
        opBQ = pos.white_bishops | pos.white_queens;
        king_square = bitscan(pos.black_king, 0);
    }
    U64 occ = ~pos.empty_squares;
    U64 own_pieces = pieces(pos, pos.turn);

    // loop rooks attackers
    U64 pinner = xray_rook_attacks(occ, own_pieces, king_square) & opRQ;
    while(pinner)
    {
        square pinner_sq = bitscan(pinner, 0);    
        pinned |= sq_rook_attacks(occ, pinner_sq) & sq_rook_attacks(occ, king_square) & own_pieces;
        pinner &= pinner - 1;
    }
    
    // loop bishops attackers
    pinner = xray_bishop_attacks(occ, own_pieces, king_square) & opRQ;
    while(pinner)
    {
        square pinner_sq = bitscan(pinner, 0);    
        pinned |= sq_bishop_attacks(occ, pinner_sq) & sq_bishop_attacks(occ, king_square) & own_pieces;
        pinner &= pinner - 1;
    }
    return pinned;
}

