//
// Created by Henok Jackson on 29/05/26.
//

#ifndef SCNAKE_FOOD_H
#define SCNAKE_FOOD_H

#include "common.h"
#include <stdint.h>
#include <time.h>

typedef struct food {
    position position;
    bool is_superfood;
    uint8_t reward;
    uint8_t size;
    time_t spawned_time;
} food;

void initialize_food(food* p_food);

#endif //SCNAKE_FOOD_H