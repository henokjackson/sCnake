//
// Created by Henok Jackson on 06/06/26.
//

#ifndef SCNAKE_COMMON_H
#define SCNAKE_COMMON_H

#include <stdint.h>
#define VIEWPORT_WIDTH 8
#define VIEWPORT_HEIGHT 8

typedef enum pixel_type{
    BLANK,
    SNAKE_BODY,
    SNAKE_HEAD_UP,
    SNAKE_HEAD_DOWN,
    SNAKE_HEAD_LEFT,
    SNAKE_HEAD_RIGHT,
    FOOD,
    BORDER
} pixel_type;

typedef struct position {
    uint8_t x_coordinate;
    uint8_t y_coordinate;
} position;

typedef struct dll_position {
    position position;
    struct dll_position* prev;
    struct dll_position* next;
} dll_position;

unsigned int get_random_number(int min, int max, unsigned int seed);

void set_random_position(
    const position* arr_permitted_coordinates,
    const uint8_t permitted_coordinates_arr_size,
    position* p_position);

extern pixel_type frame_buffer[VIEWPORT_HEIGHT][VIEWPORT_WIDTH];

void set_frame_buffer(const dll_position* coordinates);

void flush_frame_buffer();

void render_frame_buffer();

#endif //SCNAKE_COMMON_H