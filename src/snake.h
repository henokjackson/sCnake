//
// Created by Henok Jackson on 29/05/26.
//

#ifndef SCNAKE_SNAKE_H
#define SCNAKE_SNAKE_H

#include "common.h"

typedef enum direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction;

typedef struct snake {
    uint8_t size;
    direction direction;
    dll_position* head;
    dll_position* tail;
} snake;

void set_random_direction(direction* snake_direction);

#endif //SCNAKE_SNAKE_H