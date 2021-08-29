#include <stdio.h>
#include "../include/bitboard.h"
#include "../include/bitboard_attacks.h"
#include "../include/square_attacks.h"
#include "../include/game_utility.h"

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

    position* pos = create_starting_position();
    pos->black_bishops = (1ULL << f3);
    update_empty_squares(pos);

    Move moves[1024];

    generate_legal_moves(*pos, moves);

    free(pos);
	return 0;
}

void init_engine()
{
    precalc_attacks(arr_king_attacks);
	init_rays_attacks();
}
