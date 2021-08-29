#pragma once

#include "bitboard.h"
#include "bitboard_attacks.h"
#include "square_attacks.h"

#define _MASK_BIT(x, b) ((1ULL << (b)) & (x))

enum move_type
{
    QUIET, CAPTURES, EN_PASS
};

typedef struct Move
{
    square from, to;
    byte type;
    byte piece;
} Move;

U64 find_absolute_pins(position pos);

void print_move(Move move);

position make_move(position pos, Move move, byte piece);

byte is_check(position pos);

// return number of generated moves
int generate_legal_moves(position pos, Move* arr);

