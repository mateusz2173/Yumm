#ifndef MOVE_H
#define MOVE_H

#include "board.h"

typedef struct
{
    byte from, to;
    piece piece_from, piece_to, piece_captured;
    byte flags;
    byte castle;
} move;

char* str(move m, char* str);

#endif