//
// Created by Henok Jackson on 29/05/26.
//

#include "food.h"
#include "common.h"

void initialize_food(food* p_food) {
    const position* arr_available_coordinates = generate_available_coordinates_list(0, 0);
    set_random_position(arr_available_coordinates, 1, &p_food -> position);
}