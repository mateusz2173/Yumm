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

void serialize_move(Move move, char buffer[])
{
    int counter = 0;
    if(move.piece == PAWN)
    {
        if(move.type == CAPTURES)
        {
            const char* from_sq = SQ_NAMES[move.from];
            buffer[counter++] = from_sq[0];
            buffer[counter++] = 'x';
        }
    }
    else
    {
        buffer[counter++] = PIECE_NAMES[white][move.piece];    
        if(move.type == CAPTURES)
            buffer[counter++] = 'x';
    }
    const char* to_sq = SQ_NAMES[move.to];
    buffer[counter++] = to_sq[0];
    buffer[counter++] = to_sq[1];
    buffer[counter++] = '\0';
}

byte is_check(position pos)
{
    square king_sq;
    if(pos.turn == white)
        king_sq = bitscan(pos.white_king, 0);
    else
        king_sq = bitscan(pos.black_king, 0);
    return attacks_to_sq(pos, king_sq, !pos.turn) != 0; 
}

byte is_double_check(position pos)
{
    return 1;
}

int generate_legal_moves(position pos, Move* arr)
{
    int move_count = 0;
    square king_sq;
    U64 pawns, rooks, knights, bishops, queens;
    if(pos.turn == white)
    {
        king_sq = bitscan(pos.white_king, 0);
        pawns = pos.white_pawns;
        rooks = pos.white_rooks;
        knights = pos.white_knights;
        bishops = pos.white_bishops;
        queens = pos.white_queens;
    }
    else
    {
        king_sq = bitscan(pos.black_king, 0);
        pawns = pos.black_pawns;
        rooks = pos.black_rooks;
        knights = pos.black_knights;
        bishops = pos.black_bishops;
        queens = pos.black_queens;
    }
    U64 not_own_pieces = ~pieces(pos, pos.turn);
    U64 enemy_pieces = pieces(pos, !pos.turn);
    U64 occ = ~pos.empty_squares;
    U64 own_BQ = bishops | queens;
    U64 own_RQ = rooks | queens;
    U64 attacked_squares = bb_attacks_by(pos, !pos.turn);

    U64 attackers = attacks_to_sq(pos, king_sq, !pos.turn); 
    if(!attackers) //no checks
    {
        while(pawns)
        {
            square from = bitscan(pawns, 0); 
            byte rank = (from / 8) + 1;

            U64 pawn_attacks = arr_pawn_attacks[pos.turn][from] & (enemy_pieces | square_to_bitboard(pos.enpass_square));
            while(pawn_attacks)
            {
                square to = bitscan(pawn_attacks, 0);         
                Move move = {from, to, CAPTURES, PAWN};
                arr[move_count++] = move;
                pawn_attacks &= pawn_attacks - 1;
            }

            U64 pawn_single_pushes = arr_pawn_single_pushes[pos.turn][from] & pos.empty_squares;
            U64 pawn_double_pushes = arr_pawn_double_pushes[pos.turn][from] & pos.empty_squares;
            if(pos.turn == white)
                pawn_double_pushes &= north(pos.empty_squares);
            else 
                pawn_double_pushes &= south(pos.empty_squares);
            U64 pawn_pushes = pawn_single_pushes | pawn_double_pushes;
            while(pawn_pushes)
            {
                square to = bitscan(pawn_pushes, 0); 
                Move move = {from, to, QUIET, PAWN};
                arr[move_count++] = move;
                pawn_pushes &= pawn_pushes - 1;
            }
            

            pawns &= pawns - 1;
        }
        while(knights)
        {
            square from = bitscan(knights, 0); 
            U64 bb_to = arr_knight_attacks[from] & not_own_pieces;
            while(bb_to)
            {
                square to = bitscan(bb_to, 0);         
                byte type;
                if(_MASK_BIT(enemy_pieces, to))
                {
                    type = CAPTURES;
                }
                else
                {
                    type = QUIET;
                }
                Move move = {from, to, type, KNIGHT};
                arr[move_count++] = move;
                bb_to &= bb_to - 1;
            }
            knights &= knights - 1;
        }
        while(own_BQ)
        {
            square from = bitscan(own_BQ, 0); 
            U64 bb_to = sq_bishop_attacks(occ, from) & not_own_pieces;
            while(bb_to)
            {
                square to = bitscan(bb_to, 0);         
                byte type;
                if(_MASK_BIT(enemy_pieces, to))
                {
                    type = CAPTURES;
                }
                else
                {
                    type = QUIET;
                }
                byte piece;
                if(_MASK_BIT(bishops, from))
                {
                    piece = BISHOP;
                }
                else
                {
                    piece = QUEEN;
                }
                Move move = {from, to, type, piece};
                arr[move_count++] = move;
                bb_to &= bb_to - 1;
            }
            own_BQ &= own_BQ - 1;
        }
        while(own_RQ)
        {
            square from = bitscan(own_RQ, 0); 
            U64 bb_to = sq_rook_attacks(occ, from) & not_own_pieces;
            while(bb_to)
            {
                square to = bitscan(bb_to, 0);         
                byte type;
                if(_MASK_BIT(enemy_pieces, to))
                {
                    type = CAPTURES;
                }
                else
                {
                    type = QUIET;
                }
                byte piece;
                if(_MASK_BIT(rooks, from))
                {
                    piece = ROOK;
                }
                else
                {
                    piece = QUEEN;
                }
                Move move = {from, to, type, piece};
                arr[move_count++] = move;
                bb_to &= bb_to - 1;
            }
            own_RQ &= own_RQ - 1;
        }

        // king moves
        U64 bb_to = arr_king_attacks[king_sq]  & not_own_pieces;
        while(bb_to)
        {
            square to = bitscan(bb_to, 0);         
            byte type;
            if(_MASK_BIT(enemy_pieces, to))
            {
                type = CAPTURES;
            }
            else
            {
                type = QUIET;
            }
            Move move = {king_sq, to, type, KING};
            arr[move_count++] = move;
            bb_to &= bb_to - 1;
        }
    }
    if((attackers & (attackers - 1)))  //double check
    {

    }
    else //single check
    {

    }
    
    for(int i = 0; i < move_count; ++i)
    {
        char buffer[10];
        serialize_move(arr[i], buffer);
        printf("%d. %s\n", i, buffer);
    }

    return 1;
}

position make_move(position pos, Move move, byte piece)
{
    return pos;
}
