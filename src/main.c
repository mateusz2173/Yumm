#include <stdio.h>
#include "../include/bitboard.h"
#include "../include/bitboard_attacks.h"
#include "../include/square_attacks.h"

#define _PRINT(x) printf(#x" = %d\n", x)
#define _PRINTSTR(x) printf(#x" = %s\n", x)
#define _PRINTHEX(x) printf(#x" = %x\n", x)
#define _NEWLINE() printf("\n")
#define _PRINTBOARD(b) printf(#b" => \n"); print_chess_board(b)

void init_engine();
U64 pins();

int main()
{
    init_engine();

    position pos = {0};

    pos.turn = black;
    pos.black_king = square_to_bitboard(f8);
    pos.white_king = square_to_bitboard(a1);
    pos.white_rooks = square_to_bitboard(b8);
    pos.black_pawns = square_to_bitboard(d8);
    pos.empty_squares = ~(pos.black_king | pos.white_king | pos.white_rooks | pos.black_pawns);

    _PRINTBOARD(pins(pos));

	return 0;
}

void init_engine()
{
    precalc_king_attacks(arr_king_attacks);
	init_rays_attacks();
}
