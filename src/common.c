//
// Created by Henok Jackson on 06/06/26.
//

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void flush_frame_buffer() {
    memset(frame_buffer, BLANK, sizeof(frame_buffer));
}

void set_frame_buffer(const dll_position* coordinates) {
    flush_frame_buffer();
    for(dll_position* coordinate_iterator = coordinates;
        coordinate_iterator != NULL;
        coordinate_iterator = coordinate_iterator -> next) {
        frame_buffer[coordinate_iterator -> position.x_coordinate][coordinate_iterator -> position.y_coordinate] = true;
    }
}

void render_frame_buffer() {
    wchar_t pixel = ' ';
    for(int i = 0; i < VIEWPORT_HEIGHT; i++) {
        for(int j = 0; j < VIEWPORT_WIDTH; j++) {
            if (frame_buffer[i][j] == SNAKE_BODY) {
                pixel = L'■';
            }
            if (frame_buffer[i][j] == SNAKE_HEAD_UP) {
                pixel = L'▲';
            }
            if (frame_buffer[i][j] == SNAKE_HEAD_DOWN) {
                pixel = L'▼';
            }
            if (frame_buffer[i][j] == SNAKE_HEAD_LEFT) {
                pixel = L'◀';
            }
            if (frame_buffer[i][j] == SNAKE_HEAD_RIGHT) {
                pixel = L'▶';
            }
            if (frame_buffer[i][j] == FOOD) {
                pixel = L'●';
            }
            printf("%d ", frame_buffer[i][j]);
        }
    }
}

unsigned int get_random_number(const int min, const int max, const unsigned int seed) {
    srand(seed);
    const int random_number = (rand() % (max - min + 1)) + min;

    return random_number;
}

void set_random_position(
    const position* arr_permitted_coordinates,
    const uint8_t permitted_coordinates_arr_size,
    position* p_position) {
    /**
     * INFO:
     *      While generating random coordinates there is a thing to consider. Generating a random coordinate by excluding some is
     *      very difficult. It involves using very clumsy logic to increment / decrement the generated random number to another one.
     *      There are a few practical ways to tackle this but they come with their own set of challenges:
     *
     *  ->  The first idea is to pass in a list of excluded coordinates. When we say coordinates it is a pair.
     *      There are multiple ways to pass in the data. Either as a 2-D boolean array of size VIEWPORT_WIDTH x VIEWPORT_HEIGHT, where
     *      the element in the location [exclude_x][exlude_y] will be marked true, else false. But this would result in a sparse matrix,
     *      wasting stack space. Also using an array is restrictive as the size is fixed and the snake body size might not take up the whole
     *      array space so that gets wasted. However, this is easy to randomize as the random function just needs to pick the index of any of
     *      those elements. But there is a catch, even if the index selection is randomized, there still is a chance that it has too keep
     *      generating a random number until it finds the one element that is marked false. That would first require the sparse matrix to be
     *      transformed into an array of the ones with only false elements which adds in more complexity. The only advantage here is the index.
     *      For this case, the time complexity would involve
     *
     *  ->  Second approach is to use a linked list this would take up lesser space as the linked list will take up the length of the snake
     *      and the food coordinates. The issue here is randomization. As the nodes of a linked list are not indexed it is difficult to access
     *      a particular element in O(1). However, using a generated random number, a for loop can be used to traverse the linked list. So the
     *      linked list must contain only the non-excluded ones. That makes it faster and smaller.
     *
     *  ->  Conclusion: I don't understand why I even thought about the array idea. The coordinates to be excluded are of the snake and of the food
     *      which are already a linked list. So, I'll go witht he linked list method.
     */

    // generate random index to select a coordinate from a list of permitted coordinates.
    constexpr int random_index_min = 0;
    const int random_index_max = permitted_coordinates_arr_size;
    const unsigned int random_index = get_random_number(random_index_min, random_index_max, time(nullptr));

    // set new random position
    p_position -> x_coordinate = arr_permitted_coordinates[random_index].x_coordinate;
    p_position -> y_coordinate = arr_permitted_coordinates[random_index].y_coordinate;
}