#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef uint64_t U64;
typedef uint8_t byte;
typedef byte square;

#define CASTLE_WK 1
#define CASTLE_WQ 2
#define CASTLE_BK 4
#define CASTLE_BQ 8

#define _MASK_BIT(x, b) ((1ULL << (b)) & (x))

enum color
{
	WHITE, BLACK, NONE
};

enum squares 
{
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};

enum pieces
{
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY
};

typedef int UTFCHAR;

extern const char SQ_NAMES[64][3];
extern const char PIECE_NAMES[2][6];
extern const wchar_t PIECE_CHARS[2][6]; 

typedef struct
{
    U64 pieces[2][6];
	U64 empty_squares;
    U64 pinned;
	int draw_move_counter;
	byte turn;
	byte castle_perm;
	byte enpass_square;
} position;

position* create_starting_position();
U64 pieces(position pos, byte color);
void update_empty_squares(position* pos);
position parse_fen(const char* fen);

byte piece_on_square(position pos, square sq);
byte color_on_square(position pos, square sq);

extern const U64 not_A_file;
extern const U64 not_H_file;

extern const U64 rank4;
extern const U64 rank5;

square bitscan_reverse(U64 bb);
square bitscan(U64 bb, byte reverse);

U64 square_to_bitboard(square sq);

U64 north(U64 b);
U64 south(U64 b);
U64 east(U64 b); 
U64 west(U64 b);
U64 north_east(U64 b);
U64 south_east(U64 b);
U64 south_west(U64 b);
U64 north_west(U64 b);

void print_board(U64 b);
void print_chess_board(U64 b);
void print_position(position pos);
