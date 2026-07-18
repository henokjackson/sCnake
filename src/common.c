//
// Created by Henok Jackson on 06/06/26.
//
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void flush_frame_buffer() {
    memset(frame_buffer, BLANK, sizeof(frame_buffer));
}

void set_frame_buffer(const dll_pixel* coordinates) {
    if (coordinates == nullptr) {
        perror("Null pointer dereference !");
        exit(EXIT_FAILURE);
    }

    flush_frame_buffer();
    for(dll_pixel* coordinate_iterator = coordinates;
        coordinate_iterator != NULL;
        coordinate_iterator = coordinate_iterator -> next) {
        frame_buffer[coordinate_iterator -> position.x_coordinate][coordinate_iterator -> position.y_coordinate] = coordinate_iterator -> type;
    }
}

void render_frame() {
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
            printf("%lc", pixel);
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
    if (arr_permitted_coordinates == nullptr ||
        p_position == nullptr) {
        perror("Possible null pointer dereference !");
        exit(EXIT_FAILURE);
    }
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

position* generate_available_coordinates_list(const uint8_t snake_size, const uint8_t food_size) {
    /**
     * INFO:
     *  There's yet one another important think that I absolutely missed !
     *  The loop below just makes a list of coordinates of the snake body ! - why do I even need that ??!!
     *  What I want is a list of available coordinates, coordinates that are not occupied by the snake body
     *  or by the food or the ones that match the border condition depending on direction.
     *
     *  There are several approaches popping up in my head !!
     *
     *  1. The first approach would be to iterate through each and every one coordinated in the plane and
     *  for each coordinate, iterste through the snake identify the correct coordinates. But dawg !! that's
     *  gonna have a huge time complexity, nearly O(n^2). I think there are better ways !
     *
     *  2. The second approach would be to use the first ever idea I had, that is to store a matrix of size
     *  VIEWPORT_WIDTH x VIEWPORT_HEIGHT. Since arrays can be dynamically allocated, it is flexible and can be
     *  used easioy for this purpose as the matrix will be a boolean array and the occupied coordinates will be
     *  marked true. The only issue would be updating the matrix each time.
     *  NOTE: The sad part here is that, there might not be a need for a linked list for representing a snake
     *  at all. The array solution might solve the problem automatically. Sometimes even none of these might be
     *  required, Just some clever bit shifting is all I need maybe !!
     *
     *  One way to tackle this is by thinking how the frames will be rendered. In order to render each frame,
     *  I need to run a loop O(n^2) it's same as running a nxn sized array linearly..so flat / non-flat
     *  doesn't really matter. If I'm using a linked list, for each coordinate in the array, I will have to
     *  iterate the whole snake body. How about reversing it? How about iterating the snake and setting each
     *  element in the array and then printing it. Okay great! so the frame render issue is solved and we need
     *  to use the 2-D array either way.
     *
     *  From my research I found out that the best thing is either to use a static array which will be initialized
     *  based on the viewport size based on commandline argument or terminal screen size.So let's consider
     *  the array approach first !
     */

    /*TODO: Don't forget to free this piece of shit !*/
    position* arr_available_coordinates = malloc((VIEWPORT_HEIGHT * VIEWPORT_WIDTH - snake_size - food_size) * sizeof(position));
    if (arr_available_coordinates == nullptr) {
        perror("Heap memory full !");
        exit(EXIT_FAILURE);
    }

    // Iterate the array and get a list of all the cells that are marked `false`.
    for (int i = 0; i < VIEWPORT_HEIGHT; i++) {
        for (int j = 0; j < VIEWPORT_WIDTH; j++, arr_available_coordinates++) {
            if (frame_buffer[i][j] == BLANK) {
                arr_available_coordinates -> x_coordinate = j;
                arr_available_coordinates -> y_coordinate = i;
            }
        }
    }
    return arr_available_coordinates;
}