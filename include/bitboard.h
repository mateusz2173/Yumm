#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t U64;
typedef uint8_t byte;
typedef byte square;

#define CASTLE_WK 1
#define CASTLE_WQ 2
#define CASTLE_BK 4
#define CASTLE_BQ 8

enum color
{
	white, black
};

typedef struct
{
	U64 white_pawns, white_rooks, white_knights, white_bishops, white_king, white_queens;
	U64 black_pawns, black_rooks, black_knights, black_bishops, black_king, black_queens;
	U64 empty_squares;
	int draw_move_counter;
	byte turn;
	byte castle_perm;
	byte enpass_square;
} position;

position* create_starting_position();

square bitscan_forward(U64 bb);

U64 north(U64 b);
U64 south(U64 b);
U64 east(U64 b); 
U64 west(U64 b);
U64 north_east(U64 b);
U64 south_east(U64 b);
U64 south_west(U64 b);
U64 north_west(U64 b);

void print_board(U64 b);


