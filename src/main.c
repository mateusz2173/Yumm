#include <stdio.h>
#include "../include/bitboard.h"
#include "../include/move.h"

#define _PRINT(x) printf(#x" = %d\n", x)
#define _PRINTSTR(x) printf(#x" = %s\n", x)
#define _PRINTHEX(x) printf(#x" = %x\n", x)
#define _NEWLINE() printf("\n")

int main()
{
	position* pos = create_starting_position();
	precalc_king_attacks(arr_king_attacks);
	init_rays_attacks();

	print_chess_board(rays_attacks[a1][W]);

	free(pos);
	return 0;
}
