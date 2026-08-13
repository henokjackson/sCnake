#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "food.h"
#include "snake.h"

static void set_random_direction(direction* snake_direction) {
    const unsigned int random_number = get_random_number(1, sizeof(direction[0]), time(nullptr));
    *snake_direction = (direction)random_number;
}

static void set_border_coordinates(const enum direction direction) {
    /**
     * INFO:
     *  There is one thing that I missed out. Since, initial size of the snake will be 2,
     *  the snake head can only be placed outside the border as the tail has to be fit in the same row/column.
     *  Hence I need to write some logic to filter out the border pixels.
     *  The main issue here is that all of these depends on the initial direction chosen.
     *  Hence the direction needs to be decided and then the list of available coordinates will be generated
     *  based on that. The issue here is that I don't want the snake head and tail to be in two different places,
     *  which looks kinda weird. So the first thing is to bring determinsm by deciding the direction and then
     *  deciding the position based on the excluded one.
     *
     *  After careful experimentation I came up with this logic:
     *  Left  -> Exclude all coordinates of rightmost column.
     *  Right -> Exclude all coordinates of leftmost column.
     *  Up    -> Exclude all coordinates of bottomest column.
     *  Down  -> Exclude all coordinates of uppermost column.
     */
    for (int i = 0; i < VIEWPORT_HEIGHT; i++) {
        for (int j = 0; j < VIEWPORT_WIDTH; j++) {
            if ((direction == LEFT && j == 0) ||
                (direction == DOWN && i == 0) ||
                (direction == RIGHT && j == VIEWPORT_WIDTH - 1) ||
                (direction == UP && i == VIEWPORT_HEIGHT - 1)) {
                frame_buffer[i][j] = BORDER;
            }
        }
    }
}

static dll_pixel* get_snake_coordinates_list(const snake* p_snake) {
    if (p_snake == nullptr) {
        perror("Possible null pointer dereference !");
        exit(EXIT_FAILURE);
    }

    /**
     * The elements in the linked list are stored in order.
     * The first element is the head and tail is the element in the list.
     */
    auto snake_coordinates_list = (dll_pixel*)malloc(sizeof(dll_pixel) * p_snake -> size);
    if (snake_coordinates_list == nullptr) {
        perror("Heap memory full !");
        exit(EXIT_FAILURE);
    }

    for(dll_pixel* snake_cell_iterator = p_snake -> head;
        snake_cell_iterator != nullptr;
        snake_cell_iterator = snake_cell_iterator -> next) {
        snake_coordinates_list -> position = snake_cell_iterator -> position;
        snake_coordinates_list = snake_coordinates_list -> next;
        snake_cell_iterator = snake_cell_iterator -> next;
    }
    return snake_coordinates_list;
}

static void set_snake_head_pixel_type(const snake* p_snake) {
    if (p_snake == nullptr || p_snake -> head == nullptr) {
        perror("Possible null pointer dereference !");
        exit(EXIT_FAILURE);
    }

    switch (p_snake -> direction) {
        case LEFT:
            p_snake -> head -> type = SNAKE_HEAD_LEFT;
            break;
        case RIGHT:
            p_snake -> head -> type = SNAKE_HEAD_RIGHT;
            break;
        case UP:
            p_snake -> head -> type = SNAKE_HEAD_UP;
            break;
        case DOWN:
            p_snake -> head -> type = SNAKE_HEAD_DOWN;
            break;
        default:
            break;
    }
}

static void set_snake_tail_coordinate(const snake* p_snake) {
    if (p_snake == nullptr || p_snake -> tail == nullptr) {
        perror("Possible null pointer dereference !");
        exit(EXIT_FAILURE);
    }
    /**
    * INFO:
    *  When the direction is already decided and the head is already decided, the tail can also be decided on
    *  these two. So, the position of tail is a function of the head position and the initial direction.
    *  tail position = f(head_position,direction).
    *
    *  The logic is pretty straightforward:
    *
    *  Left  -> Food Position = (x+1, y)
    *  Right -> Food Position = (x-1, y)
    *  Up    -> Food Position = (x, y-1)
    *  Down  -> Food Position = (x, y+1)
    */
    switch (p_snake -> direction) {
        case UP:
            p_snake -> tail -> position.y_coordinate -= 1;
            break;
        case DOWN:
            p_snake -> tail -> position.y_coordinate += 1;
            break;
        case LEFT:
            p_snake -> tail -> position.x_coordinate += 1;
            break;
        case RIGHT:
            p_snake -> tail -> position.x_coordinate -= 1;
            break;
        default:
            break;
    }
}

/**
 * TODO:
 *  Write a factory function that malloc()s the memory for snake and also
 *  increments the size along with it to avoid mistakes.
 */
static dll_pixel* create_snake_body_cell(uint8_t* snake_size) {
    dll_pixel* snake_body_cell = (dll_pixel*)malloc(sizeof(dll_pixel));
    if (snake_body_cell == nullptr) {
        perror("Heap memory full !");
        exit(EXIT_FAILURE);
    }
    (*snake_size)++;
    return snake_body_cell;
}
static void create_snake_head(snake* p_snake) {
    p_snake -> head = create_snake_body_cell(&p_snake -> size);
    p_snake -> head -> prev = nullptr;
    p_snake -> head -> next = p_snake -> tail;
}
static void create_snake_tail(snake* p_snake) {
    p_snake -> tail = create_snake_body_cell(&p_snake -> size);
    p_snake -> tail -> prev = p_snake -> head;
    p_snake -> tail -> next = nullptr;
    p_snake -> tail -> type = SNAKE_TAIL;
}

int initialize_snake(snake* p_snake, const food* p_food) {
    if (p_snake == nullptr || p_food == nullptr) {
        perror("Possible null pointer dereference !");
        exit(EXIT_FAILURE);
    }

    // initialize snake size.
    p_snake -> size = 0;

    // initialize food size.
    const uint8_t food_size = p_food -> size;

    // initialize available coordinates.
    position* arr_available_coordinates = nullptr;

    // initialize snake coordinates list.
    dll_pixel* snake_coordinates_list = nullptr;

    // create snake head.
    create_snake_head(p_snake);

    // create snake tail.
    create_snake_tail(p_snake);

    // Calculate the initial position for snake head.
    // generate list of available co-ordinates
    arr_available_coordinates = generate_available_coordinates_list(p_snake -> size, food_size);

    // set random initial position for snake head.
    set_random_position(arr_available_coordinates, p_snake -> size, &p_snake -> head -> position);

    // set random initial direction for snake head.
    set_random_direction(&p_snake -> direction);

    // set pixel_type for snake head based on direction.
    set_snake_head_pixel_type(p_snake);

    // get list of coordinates of snake pixels that includes the head.
    snake_coordinates_list = get_snake_coordinates_list(p_snake);

    // set the snake along with the head on frame buffer.
    set_frame_buffer(snake_coordinates_list);

    // Calculate the initial position for tail using direction.
    // First, exclude the border coordinates.
    set_border_coordinates(p_snake -> direction);

    // Remove the pre-allocated coordinates list.
    free((void*)arr_available_coordinates);

    // Second, exclude the head coordinate.
    // in order to remove this function call, It needs to be guaranteed that the previous
    // functions will position the snake head in such a way that the tail can be positioned
    // in any of the surrounding cross pixels
    arr_available_coordinates = generate_available_coordinates_list(p_snake -> size, food_size);

    // set snake tail position
    set_snake_tail_coordinate(p_snake);

    // get list of coordinates of snake pixels that includes the head and tail.
    snake_coordinates_list = get_snake_coordinates_list(p_snake);

    // set the snake along with the head and tail on frame buffer.
    set_frame_buffer(snake_coordinates_list);

    return 0;
}
