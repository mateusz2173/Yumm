#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>
#include "../include/bitboard.h"
#include "../include/bitboard_attacks.h"
#include "../include/square_attacks.h"
#include "../include/game_utility.h"

#define _PRINT(x) printf(#x" = %d\n", x)
#define _PRINTSTR(x) printf(#x" = %s\n", x)
#define _PRINTHEX(x) printf(#x" = %x\n", x)
#define _NEWLINE() printf("\n")
#define _PRINTBOARD(b) printf(#b" => \n"); print_chess_board(b)

#define ENGINE_NAME "YUMM"

const char* STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";
const char* EMPTY_FEN = "8/8/8/8/8/8/8/8 w";
const char* MATE_FEN = "rnbqkbnr/1ppp1Qp1/p6p/4p3/2B1P3/8/PPPP1PPP/RNB1K1NR b";
const char* DOUBLE_CHECK_FEN = "rnbqk1nr/1pp3p1/p4N2/1B6/4P3/8/PPPP1PpP/RNBQK2R b KQkq";
const char* BLOCK_CHECK_FEN = "rnbqk1nr/1pp3p1/p7/1B6/4P3/8/PPPP1PpP/RNBQK2R b KQkq";

void init_engine();
position make_random_move(position pos);

int main()
{
    init_engine();

    position pos = parse_fen(BLOCK_CHECK_FEN);

    print_position(pos);

    make_random_move(pos);

	return 0;
}

void init_engine()
{
    setlocale(LC_CTYPE, "");
    srand(time(NULL));
    precalc_attacks(arr_king_attacks);
	init_rays_attacks();
}

position make_random_move(position pos)
{
    Move arr[64];
    int len = generate_legal_moves(pos, arr);

    int r = rand() % len;
    char buff[5];
    serialize_move(arr[r], buff);
    printf("Random generated move: %s\n", buff);

    return pos;
}
