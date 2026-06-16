//
// Created by Henok Jackson on 29/05/26.
//

#ifndef SCNAKE_SNAKE_H
#define SCNAKE_SNAKE_H

#include "common.h"

enum direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct snake {
    uint8_t size;
    enum direction direction;
    struct dll_position* head;
    struct dll_position* tail;
};

void set_random_direction(enum direction* direction);

#endif //SCNAKE_SNAKE_H