#pragma once

#include "bitboard.h"
#include "bitboard_attacks.h"
#include "square_attacks.h"

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

// Returns a BB of absolute-pinned pieces
U64 find_absolute_pins(position pos);

void serialize_move(Move move, char buffer[]);

position make_move(position pos, Move move);

// returns 1 if there is a check in current position, 0 otherwise
byte is_check(position pos);


int generate_legal_moves(position pos, Move* arr);

position parse_fen(const char* fen);

