//
// Created by Henok Jackson on 06/06/26.
//

#ifndef SCNAKE_COMMON_H
#define SCNAKE_COMMON_H

#include <stdint.h>
#define VIEWPORT_WIDTH 8
#define VIEWPORT_HEIGHT 8

struct position {
    uint8_t x_coordinate;
    uint8_t y_coordinate;
};

struct dll_position {
    struct position position;
    struct dll_position* prev;
    struct dll_position* next;
};

extern bool frame_buffer[VIEWPORT_HEIGHT][VIEWPORT_WIDTH];

unsigned int get_random_number(const int min, const int max, const unsigned int seed);

void set_random_position(
    const struct position* arr_permitted_coordinates,
    const uint8_t permitted_coordinates_arr_size,
    const struct position* p_position);

void initialize_frame_buffer();

void set_frame(const struct dll_position* coordinates);

#endif //SCNAKE_COMMON_H