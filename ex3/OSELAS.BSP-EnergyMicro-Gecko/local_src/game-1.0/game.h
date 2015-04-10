#ifndef _GAME_H
#define _GAME_H

typedef enum{
    LEFT,
    UP,
    RIGHT,
    DOWN,
    STOP
}dir_t;

typedef struct position_t{
    uint32_t x;
    uint32_t y;
}position_t;

struct snake_t{
    struct position_t head_pos;
    dir_t dir_next;
    dir_t dir_last;
};

FILE *driver;
int interrupt_triggered;    //TODO:: for debugging purposes only
int gamepad_init();
int update_pos(dir_t *dir_next, position_t *head_pos);

#endif
