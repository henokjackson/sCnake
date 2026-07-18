#include <stdio.h>
#include "common.h"
#include "food.h"
#include "snake.h"

int main(int argc, char** argv) {
    /**
     * INFO: Normally, terminals open up in echo mode and we need to press ENTER key
     * for the input to be accepted by the getchar() function. The ideas is to turn off
     * echo mode using functions from termios library and call getchar to detech keypress,
     */

    // initialize frame buffer
    flush_frame_buffer();

    // initialize food
    initialize_food(nullptr);

    // initialize snake
    initialize_snake(nullptr, nullptr);

    // render frame
    render_frame();

    return 0;
}
