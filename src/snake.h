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
    dll_pixel* head;
    dll_pixel* tail;
} snake;

void initialize_food(food* p_food);

int initialize_snake(snake* p_snake, const food* p_food);

#endif //SCNAKE_SNAKE_H