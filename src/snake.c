#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "snake.h"

void set_random_direction(enum direction* direction) {
    const unsigned int random_number = get_random_number(1, sizeof(enum direction) / sizeof(enum direction[0]), time(nullptr));
    *direction = (enum direction)random_number;
}

struct position* generate_permitted_coordinates(const struct snake* p_snake) {
    /**
     * TODO: Don't forget to free this piece of shit !
     */
    struct position* arr_permitted_coordinated = malloc(p_snake -> size * sizeof(struct position));
    const struct snake_cell* snake_cell_iterator = p_snake -> head;
    /**
     * INFO:
     *  There is one thing that I missed out. Since, initial size of the snake will be 2,
     *  the snake head can only be placed outside the border as the tail has to be fit in the same row/column.
     *  Hence I need to write some logic to filter out the border pixels.
     *  The main issue here is that all of these depends on the initial direction chosen.
     *  Hence the direction needs to be decided and then the list of permitted coordinates will be generated
     *  based on that. The issue here is that I don't want the snake head and tail to be in two different places,
     *  which looks kinda weird. So the first thing is to bring determinsm by deciding the direction and then
     *  deciding the position based on the excluded one.
     *
     *  After careful experimentation I came up with this logic:
     *  Left  -> Exclude all coordinates of rightmost column.
     *  Right -> Exclude all coordinates of leftmost column.
     *  Up    -> Exclude all coordinates of bottomest column.
     *  Down  -> Exclude all coordinates of uppermost column.
     *
     *  There could be a problem whose solutions solves the other problems too.
     *
     *  When the direction is already decided and the head is already decided, the tail can also be decided on
     *  these two. So, the position of tail is a function of the head poistion and the initial direction.
     *  tail position = f(head_position,direction).
     *
     *  The logic is pretty straightforward:
     *
     *  Left  -> Food Position = (x+1, y)
     *  Right -> Food Position = (x-1, y)
     *  Up    -> Food Position = (x, y-1)
     *  Down  -> Food Position = (x, y+1)
     */
    for (int i = 0; i < p_snake -> size; i++) {
        arr_permitted_coordinated[i].x_coordinate = snake_cell_iterator -> position.x_coordinate;
        arr_permitted_coordinated[i].y_coordinate = snake_cell_iterator -> position.y_coordinate;
        snake_cell_iterator = snake_cell_iterator -> next;
    }

    return arr_permitted_coordinated;
}

void initialize_snake(struct snake* p_snake) {
    // set initial size.
    p_snake -> size = 0;

    // create snake head.
    p_snake -> head = (struct snake_cell*)malloc(sizeof(struct snake_cell));
    p_snake -> head -> prev = nullptr;
    p_snake -> head -> next = p_snake -> tail;
    p_snake -> size++;

    // create snake tail.
    p_snake -> tail = (struct snake_cell*)malloc(sizeof(struct snake_cell));
    p_snake -> tail -> prev = p_snake -> head;
    p_snake -> tail -> next = nullptr;
    p_snake -> size++;

    // generate list of permitted co-ordinates
    struct position* arr_permitted_coordinates = generate_permitted_coordinates(p_snake);

    // set random initial position for snake head.
    set_random_position(arr_permitted_coordinates, p_snake -> size, &p_snake -> head -> position);

    // set random initial direction.
    set_random_direction(&p_snake -> direction);

    // calculate initial position for tail using direction.
    set_p_snake -> tail -> position
}