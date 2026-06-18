//
// Created by Henok Jackson on 29/05/26.
//

#ifndef SCNAKE_FOOD_H
#define SCNAKE_FOOD_H

#include <stdint.h>
#include <time.h>
#include "common.h"

typedef struct food {
    position position;
    bool is_superfood;
    uint8_t reward;
    uint8_t size;
    time_t spawned_time;
} food;

#endif //SCNAKE_FOOD_H