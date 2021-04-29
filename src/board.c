#include "../include/board.h"

const char sq_names[64][3] = 
{
    "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8",
    "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8",
    "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8",
    "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8",
    "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8",
    "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8",
    "g1", "g2", "g3", "g4", "g5", "g6", "g7", "g8",
    "h1", "h2", "h3", "h4", "h5", "h6", "h7", "h8"
};

BOARD board = (BYTE[])
{
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};

BOARD starting_board = (BYTE[])
{
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0x04,0x02,0x03,0x05,0x06,0x03,0x02,0x04,0xFF,
		0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,
		0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
		0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
		0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
		0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
		0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xFF,
		0xFF,0x84,0x82,0x83,0x85,0x86,0x83,0x82,0x84,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};

const int mailbox[120] = 
{
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
     -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
     -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
     -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
     -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
     -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
     -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
     -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

const int mailbox64[64] = 
{
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
};

BOARD create_board()
{
		BOARD buff = (BOARD) malloc(sizeof(PIECE) * 125);

		return buff;
}

void set_starting_pos(BOARD board)
{
		memcpy(board, starting_board, 120);
}

PIECE piece_at(BOARD board, int sq)
{
    return board[mailbox64[sq]];
}

BYTE color(BOARD board, int sq)
{
    return piece_color(piece_at(board, sq));
}
BYTE moved(BOARD board, int sq)
{
    return piece_moved(piece_at(board, sq));
}
BYTE castle(BOARD board, int sq)
{
    return piece_castle(piece_at(board, sq));
}
BYTE type(BOARD board, int sq)
{
    return piece_type(piece_at(board, sq));
}

void print(BOARD board)
{
    for(int r = 0; r < 8; ++r)
	{
        for(int f = 0; f < 8; ++f)
        {
            int sq = r * 8 + f;

            printf("%c  ", piece_char[(int)type(board, sq)]);
        }
        printf("\n");
	}
}

