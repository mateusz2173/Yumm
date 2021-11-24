#include "../include/game_utility.h"

U64 find_absolute_pins(position pos)
{
    U64 pinned = 0ULL;
    U64 opRQ, opBQ;
    byte enemy_color = 1 - pos.turn;
    square king_square;
    opRQ = pos.pieces[enemy_color][ROOK] | pos.pieces[enemy_color][QUEEN];
    opBQ = pos.pieces[enemy_color][BISHOP] | pos.pieces[enemy_color][QUEEN];
    king_square = bitscan(pos.pieces[pos.turn][KING], 0);
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
    pinner = xray_bishop_attacks(occ, own_pieces, king_square) & opBQ;
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
        buffer[counter++] = PIECE_NAMES[WHITE][move.piece];    
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
    square king_sq = bitscan(pos.pieces[pos.turn][KING], 0);
    byte enemy_color = 1 - pos.turn;
    return attacks_to_sq(pos, king_sq, enemy_color) > 0; 
}

int generate_legal_moves(position pos, Move* arr)
{
    //initalization
    int move_count = 0;
    square king_sq;
    byte enemy_color = 1 - pos.turn;
    U64* own = pos.pieces[pos.turn];
    king_sq = bitscan(pos.pieces[pos.turn][KING], 0);
    U64 enemy_pieces = pieces(pos, !pos.turn);
    U64 own_pieces = pieces(pos, pos.turn);
    U64 occ = ~pos.empty_squares;
    U64 own_BQ = own[BISHOP] | own[QUEEN];
    U64 own_RQ = own[ROOK] | own[QUEEN];
    U64 attacked_squares = bb_attacks_by(pos, enemy_color);
    U64 pinned = find_absolute_pins(pos);

    U64 attackers = attacks_to_sq(pos, king_sq, enemy_color); 
    print_chess_board(attackers);
    if(!attackers) //no checks
    {
        printf("No checks!\n");
        U64 pawns = own[PAWN];
        while(pawns)
        {
            square from = bitscan(pawns, 0); 

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
            if(pos.turn == WHITE)
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
        U64 knights = own[KNIGHT];
        while(knights)
        {
            square from = bitscan(knights, 0); 
            U64 bb_to = arr_knight_attacks[from] & ~own_pieces;
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
            U64 bb_to = sq_bishop_attacks(occ, from) & ~own_pieces;
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
                if(_MASK_BIT(own[BISHOP], from))
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
            U64 bb_to = sq_rook_attacks(occ, from) & ~own_pieces;
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
                if(_MASK_BIT(own[ROOK], from))
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
        U64 bb_to = arr_king_attacks[king_sq] & ~own_pieces;
        while(bb_to)
        {
            square to = bitscan(bb_to, 0);         
            if(!_MASK_BIT(attacked_squares, to))
            {
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
            }

            bb_to &= bb_to - 1;
        }
    }
    else if((attackers & (attackers - 1)))  //double check
    {
        printf("Double check!\n");
        U64 bb_to = arr_king_attacks[king_sq] & (~own_pieces) & (~attacked_squares);
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
            printf("Generated double check runaway!\n");
        }
    }
    else //single check
    {
        printf("[Single check!]\n");
        U64 attacker_sq = bitscan(attackers, 0);

        byte piece = piece_on_square(pos, attacker_sq);
        U64 in_between, xray;
        switch(piece)
        {
            case PAWN:
                in_between = 0ULL;
                xray = 0ULL;
                break;
            case BISHOP:
                in_between = sq_bishop_attacks(occ, attacker_sq) & sq_bishop_attacks(occ, king_sq);
                xray = xray_bishop_attacks(occ, own_pieces, attacker_sq);
                break;
            case ROOK:
                in_between = sq_rook_attacks(occ, attacker_sq) & sq_rook_attacks(occ, king_sq);
                xray = xray_rook_attacks(occ, own_pieces, attacker_sq);
                break;
            case QUEEN: 
                if(_MASK_BIT(sq_bishop_attacks(occ, attacker_sq), king_sq)) //bishop-like queen check
                {
                    in_between = sq_bishop_attacks(occ, attacker_sq) & sq_bishop_attacks(occ, king_sq);
                    xray = xray_bishop_attacks(occ, own_pieces, attacker_sq);
                }
                else //rook-like queen check
                {
                    in_between = sq_rook_attacks(occ, attacker_sq) & sq_rook_attacks(occ, king_sq);
                    xray = xray_rook_attacks(occ, own_pieces, attacker_sq);
                }
                break;
            default:
                in_between = 0ULL;
                xray = 0ULL;
        }
        
        //run away from check or take attacker by king
        U64 attacked_squares_xray = attacked_squares | xray;
        U64 bb_to = arr_king_attacks[king_sq] & (~own_pieces) & (~attacked_squares_xray);
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
            printf("Generated runaway or take (king attacker) by king\n");
            bb_to &= bb_to - 1;
        }

        //take attacker by other piece 
        U64 bb_king = square_to_bitboard(king_sq);
        U64 bb_defenders = attacks_to_sq(pos, attacker_sq, pos.turn) & ~bb_king & ~pinned;

        while(bb_defenders)
        {
            square defender_sq = bitscan(bb_defenders, 0);
            byte piece = piece_on_square(pos, defender_sq);
            Move move = {defender_sq, attacker_sq, CAPTURES, piece};
            arr[move_count++] = move;
            printf("Generated take (king attacker) by non-king piece\n");
            bb_defenders &= bb_defenders - 1;
        }
        
        //block from attacks
        while(in_between)
        {
            square between_sq = bitscan(in_between, 0);
            U64 blockers = attacks_to_sq(pos, between_sq, pos.turn) & ~bb_king;
            while(blockers)
            {
                square blocker_sq = bitscan(blockers, 0);
                byte type = piece_on_square(pos, blocker_sq);
                Move move = {blocker_sq, between_sq, QUIET, type};
                arr[move_count++] = move;
                blockers &= blockers - 1;
                printf("Generated block from check!\n");
            }
            in_between &= in_between - 1;
        }
    }
    
    for(int i = 0; i < move_count; ++i)
    {
        char buffer[10];
        serialize_move(arr[i], buffer);
        printf("%d. %s\n", i, buffer);
    }

    return move_count;
}

position make_move(position pos, Move move)
{
    byte from_piece = piece_on_square(pos, move.from);
    byte to_piece = piece_on_square(pos, move.to); 

    return pos;
}

position parse_fen(const char* fen)
{
    position result = {0}; 

    int rank = 8;
    int file = 0;

    int i;
    for(i = 0; fen[i] != '\0' && fen[i] != ' '; i++)
    {
        byte mask = (rank - 1) * 8 + file;
        switch(fen[i])
        {
            case '/':
                rank--;
                file = 0;
                break;
            case '1':
                file += 1;
                break;
            case '2':
                file += 2;
                break;
            case '3':
                file += 3;
                break;
            case '4':
                file += 4;
                break;
            case '5':
                file += 5;
                break;
            case '6':
                file += 6;
                break;
            case '7':
                file += 7;
                break;
            case '8':
                file += 8;
                break;
            case 'p':
                result.pieces[BLACK][PAWN] |= square_to_bitboard(mask);
                file++;
                break;
            case 'r':
                result.pieces[BLACK][ROOK] |= square_to_bitboard(mask);
                file++;
                break;
            case 'b':
                result.pieces[BLACK][BISHOP] |= square_to_bitboard(mask);
                file++;
                break;
            case 'n':
                result.pieces[BLACK][KNIGHT] |= square_to_bitboard(mask);
                file++;
                break;
            case 'q':
                result.pieces[BLACK][QUEEN] |= square_to_bitboard(mask);
                file++;
                break;
            case 'k':
                result.pieces[BLACK][KING] |= square_to_bitboard(mask);
                file++;
                break;
            case 'P':
                result.pieces[WHITE][PAWN] |= square_to_bitboard(mask);
                file++;
                break;
            case 'R':
                result.pieces[WHITE][ROOK] |= square_to_bitboard(mask);
                file++;
                break;
            case 'B':
                result.pieces[WHITE][BISHOP] |= square_to_bitboard(mask);
                file++;
                break;
            case 'N':
                result.pieces[WHITE][KNIGHT] |= square_to_bitboard(mask);
                file++;
                break;
            case 'Q':
                result.pieces[WHITE][QUEEN] |= square_to_bitboard(mask);
                file++;
                break;
            case 'K':
                result.pieces[WHITE][KING] |= square_to_bitboard(mask);
                file++;
                break;
            default:
                fprintf(stderr, "Invaild character (Index: %d, char: %c)\n", i, fen[i]);
                return result;
        }

        if(rank < 1)
        {
            fprintf(stderr, "FEN cannot have more than 8 ranks! (Index: %d, char: %c)\n", i, fen[i]);
            return result;
        }

        if(file > 8)
        {
            fprintf(stderr, "%d squares on FEN rank. (Index: %d, char: %c)\n", file, i, fen[i]);
            return result;
        }
    }

    if(rank != 1)
    {
        fprintf(stderr, "FEN must have 8 ranks!\n");
        return result;
    }

    if(fen[i] == ' ')
    {
        switch(fen[++i])
        {
            case 'w':
                result.turn = WHITE;
                break;
            case 'b':
                result.turn = BLACK;
                break;
            case '\0':
                result.turn = WHITE;
                break;
            default:
                fprintf(stderr, "Invaild turn character. Should be 'w' or 'b' (Index: %d, char: %c)\n", i, fen[i]);
                return result;
        }
    }
    else if (fen[i] != '\0')
    {
        fprintf(stderr, "Invaild character (Index: %d, char: %c)\n", i, fen[i]);
        return result;
    }

    update_empty_squares(&result);
    result.pinned = find_absolute_pins(result);
    return result;
}
