#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define VIEWPORT_WIDTH 8
#define VIEWPORT_HEIGHT 8

enum direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct position {
    uint8_t pos_x;
    uint8_t pos_y;
    struct position* next_element;
};

struct snake_cell {
    struct snake_cell* prev_cell;
    struct snake_cell* next_cell;
    struct position pos;
};

struct snake {
    struct snake_cell* head;
    struct snake_cell* tail;
    enum direction direction;
    uint8_t size;
};

unsigned int get_random_number(int min, int max, unsigned int seed) {
    srand(seed);
    const int random_number = (rand() % (max - min + 1)) + min;

    return random_number;
}

enum direction get_random_initial_direction() {
    unsigned int random_number = get_random_number(0, sizeof(enum direction) / sizeof(enum direction[0]), time(nullptr));

    return (enum direction)random_number;
}

void set_random_pos(
    struct position* permitted_coordinates,
    const uint8_t permitted_coordinates_list_size,
    struct position* p_position) {
    /**
     * INFO: While generating random coordinates there is a thing to consider. Generating a random coordinate by excluding some is
     * very difficult. It involves using very clumsy logic to increment / decrement the generated random number to another one.
     * There are a few practical ways to tackle this but they come with their own set of challenges:
     *
     * -> 1. The first idea is to pass in a list of excluded coordinates. When we say coordinates it is a pair.
     *      There are multiple ways to pass in the data. Either as a 2-D boolean array of size VIEWPORT_WIDTH x VIEWPORT_HEIGHT, where
     *      the element in the location [exclude_x][exlude_y] will be marked true, else false. But this would result in a sparse matrix,
     *      wasting stack space. Also using an array is restrictive as the size is fixed and the snake body size might not take up the whole
     *      array space so that gets wasted. However, this is easy to randomize as the random function just needs to pick the index of any of
     *      those elements. But there is a catch, even if the index selection is randomized, there still is a chance that it has too keep
     *      generating a random number until it finds the one element that is marked false. That would first require the sparse matrix to be
     *      transformed into an array of the ones with only false elements which adds in more complexity. The only advantage here is the index.
     *      For this case, the time complexity would involve
     *
     * -> 2. Second approach is to use a linked list this would take up lesser space as the linked list will take up the length of the snake
     *      and the food coordinates. The issue here is randomization. As the nodes of a link list are not indexed it is difficult to access
     *      a particular element in O(1). However, using a generated random number, a for loop can be used to traverse the linked list. So the
     *      linked list must contain only the non-excluded ones. That makes it faster and smaller.
     *
     * -> Conclusion: I don't understand why I even thought about the array idea. The coordinates to be excluded are of the snake and of the food
     *    which are already a linked list. So, I'll go witht he linked list method.
     */

    // generate random index to select a coordinate from a list of permitted coordinates.
    int random_index_min = 0;
    int random_index_max = permitted_coordinates_list_size;
    unsigned int random_index = get_random_number(random_index_min, random_index_max, time(nullptr));

    struct position* current_position_element = permitted_coordinates;
    for (unsigned int index = 0; index == random_index; index++) {
        current_position_element = current_position_element->next_element;
    }
    // update position
    p_position -> pos_x = random_x;
    p_position -> pos_y = random_y;

    return;
}

void initialize_snake(struct snake* snake) {
    snake->head = (struct snake_cell*)malloc(sizeof(struct snake_cell));
    snake->tail = (struct snake_cell*)malloc(sizeof(struct snake_cell));

    snake->head->prev_cell = nullptr;
    snake->head->next_cell = snake->tail;

    snake->tail->prev_cell = snake->head;
    snake->tail->next_cell = nullptr;

}

void initialize_snake(struct snake* snake) {
    snake->head = (struct snake_cell*)malloc(sizeof(struct snake_cell));
    snake->tail = (struct snake_cell*)malloc(sizeof(struct snake_cell));

    snake->head->prev_cell = nullptr;
    snake->head->next_cell = snake->tail;

    snake->tail->prev_cell = snake->head;
    snake->tail->next_cell = nullptr;

    rand();
}