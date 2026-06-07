//
// Created by Henok Jackson on 29/05/26.
//

#ifndef SCNAKE_SNAKE_H
#define SCNAKE_SNAKE_H

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
    struct snake_cell* head;
    struct snake_cell* tail;
};

struct snake_cell {
    struct position position;
    struct snake_cell* prev;
    struct snake_cell* next;
};

void set_random_direction(enum direction* direction);

#endif //SCNAKE_SNAKE_H