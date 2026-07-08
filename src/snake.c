#include "common.h"
#include "food.h"
#include "snake.h"
#include <stdlib.h>

void set_random_direction(direction* snake_direction) {
    const unsigned int random_number = get_random_number(1, sizeof(direction[0]), time(nullptr));
    *snake_direction = (direction)random_number;
}

position* generate_available_coordinates_list(const uint8_t snake_size, const uint8_t food_size) {
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
     *
     *  There's yet one another important think that I absolutely missed !
     *  The loop below just makes a list of coordinates of the snake body ! - why do I even need that ??!!
     *  What I want is a list of available coordinates, coordinates that are not occupied by the snake body
     *  or by the food or the ones that match the border condition depending on direction.
     *
     *  There are several approached poping up in my head !!
     *
     *  1. The first approach would be to iterate through each and every one coordinated in the plane and
     *  for each coordinate, iterste through the snake identify the correct coordinated. But dawg !! that's
     *  gonna have an huge time complexity, nearly O(n^2). I think there are better ways !
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

    /**
     * TODO: Don't forget to free this piece of shit !
     */
    position* arr_available_coordinates = malloc((VIEWPORT_HEIGHT * VIEWPORT_WIDTH - snake_size - food_size) * sizeof(position));

    // Iterate the array and get a list of all the cells that are marked `false`.
    for (int i = 0; i < VIEWPORT_HEIGHT; i++) {
        for (int j = 0; j < VIEWPORT_WIDTH; j++, arr_available_coordinates++) {
            if (frame_buffer[i][j] == BLANK) { /** TODO: add a null pointer check here ! */
                arr_available_coordinates -> x_coordinate = j;
                arr_available_coordinates -> y_coordinate = i;
            }
        }
    }

    return arr_available_coordinates;
}

void set_border_coordinates(const enum direction direction) {
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

dll_pixel* get_snake_coordinates_list(const snake* p_snake) {
    /**
     * The elements in the linked list are stored in order. The first element is the head and tail is the
     * element in the list.
     */

    auto snake_coordinates_list = (dll_pixel*)malloc(sizeof(dll_pixel) * p_snake -> size);
    for(dll_pixel* snake_cell_iterator = p_snake -> head;
        snake_cell_iterator != nullptr;
        snake_cell_iterator = snake_cell_iterator -> next) {
        snake_coordinates_list -> position = snake_cell_iterator -> position;
        snake_coordinates_list = snake_coordinates_list -> next;
        snake_cell_iterator = snake_cell_iterator -> next;
    }

    return snake_coordinates_list;
}

void set_snake_head_pixel_type(const snake* p_snake) {
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

void set_snake_tail_coordinate(const snake* p_snake) {
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

void initialize_food(food* p_food) {
    const position* arr_available_coordinates = generate_available_coordinates_list(0, 0);
    set_random_position(arr_available_coordinates, 1, &p_food -> position);
}

int initialize_snake(snake* p_snake, const food* p_food) {
    // initialize snake size.
    p_snake -> size = 0;

    // initialize food size.
    const uint8_t food_size = p_food -> size;

    // initialize available coordinates.
    position* arr_available_coordinates = nullptr;

    // initialize snake coordinates list.
    dll_pixel* snake_coordinates_list = nullptr;

    /**
     * TODO: Write a factory function that malloc()s the memory for snake and also
     *       increments the size along with it to avoid mistakes.
     */
    // create snake head.
    p_snake -> head = (dll_pixel*)malloc(sizeof(dll_pixel));

    // create snake tail.
    p_snake -> tail = (dll_pixel*)malloc(sizeof(dll_pixel));

    // set snake head parameters.
    p_snake -> head -> prev = nullptr;
    p_snake -> head -> next = p_snake -> tail;
    p_snake -> size++;

    // set snake tail parameters.
    p_snake -> tail -> prev = p_snake -> head;
    p_snake -> tail -> next = nullptr;
    p_snake -> tail -> type = SNAKE_TAIL;
    p_snake -> size++;

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
    arr_available_coordinates = generate_available_coordinates_list(p_snake -> size, food_size);

    // set snake tail position
    set_snake_tail_coordinate(p_snake);

    // get list of coordinates of snake pixels that includes the head and tail.
    snake_coordinates_list = get_snake_coordinates_list(p_snake);

    // set the snake along with the head and tail on frame buffer.
    set_frame_buffer(snake_coordinates_list);

    return 0;
}
