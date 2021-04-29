#pragma once

#include "board.h"

typedef struct
{
    BYTE from, to;
    PIECE piece_from, piece_to, piece_captured;
    BYTE flags;
    BYTE castle;
} move;

char* str(move m, char* str);