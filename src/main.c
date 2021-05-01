#include <stdio.h>
#include "../include/bitboard.h"
#include "../include/move.h"

#define _PRINT(x) printf(#x" = %d\n", x)
#define _PRINTSTR(x) printf(#x" = %s\n", x)
#define _PRINTHEX(x) printf(#x" = %x\n", x)

int main()
{
	position* pos = create_starting_position();
	precalc_king_attacks(arr_king_attacks);

	print_board(arr_king_attacks[5]);

	free(pos);
	return 0;
}
