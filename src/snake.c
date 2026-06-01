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

void get_random_pos(const unsigned int exclude_x, const unsigned int exclude_y, struct position* p_position) {

    // generate random x axis coordinate
    int random_x_min = 0;
    int random_x_max = VIEWPORT_WIDTH - 1;
    unsigned int random_x = get_random_number(random_x_min, random_x_max, time(nullptr));
    if (random_x == exclude_x) {
        if (exclude_x == random_x_min) {
            random_x++;
        } else if (exclude_x == random_x_max) {
            random_x--;
        }
    }

    // generate random y axis coordinate
    int random_y_min = 0;
    int random_y_max = VIEWPORT_HEIGHT - 1;
    unsigned int random_y = get_random_number(random_y_min, random_y_max, time(nullptr));
    if (random_y == exclude_x) {
        if (random_y % 2 == 0) {
            random_y++;
        } else {
            random_y--;
        }#include <stdint.h>
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

void get_random_pos(const unsigned int exclude_x, const unsigned int exclude_y, struct position* p_position) {

    // generate random x axis coordinate
    int random_x_min = 0;
    int random_x_max = VIEWPORT_WIDTH - 1;
    unsigned int random_x = get_random_number(random_x_min, random_x_max, time(nullptr));
    if (random_x == exclude_x) {
        if (exclude_x == random_x_min) {
            random_x++;
        } else if (exclude_x == random_x_max) {
            random_x--;
        }
    }

    int random_y_min = 0;
    int random_y_max = VIEWPORT_HEIGHT - 1;
    unsigned int random_y = get_random_number(random_y_min, random_y_max, time(nullptr));
    if (random_y == exclude_x) {
        if (random_y % 2 == 0) {
            random_y++;
        } else {
            random_y--;
        }
    }
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
    }
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