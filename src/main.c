#include <stdio.h>
#include "../include/board.h"

#define _PRINT(x) printf(#x" = %d\n", x)
#define _PRINTSTR(x) printf(#x" = %s\n", x)
#define _PRINTHEX(x) printf(#x" = %x\n", x)

int main()
{
		BOARD board = create_board();
		set_starting_pos(board);

		print(board);

		free(board);
		return 0;
}
