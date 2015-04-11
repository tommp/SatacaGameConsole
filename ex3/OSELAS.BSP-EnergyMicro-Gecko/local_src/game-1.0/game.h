#ifndef _GAME_H
#define _GAME_H

#define GRID_HEIGHT     SCREEN_HEIGHT/GAME_PIXEL_SIZE
#define GRID_WIDTH      SCREEN_WIDTH/GAME_PIXEL_SIZE

#define CELL_FREE       0
#define CELL_BLOCKED    1

#define TIME_PER_LOOP   100
#define NUM_PLAYERS     2

#define SW_8            0b10000000
#define SW_7            0b01000000
#define SW_6            0b00100000
#define SW_5            0b00010000
#define SW_4            0b00001000
#define SW_3            0b00000100
#define SW_2            0b00000010
#define SW_1            0b00000001

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

struct player_t{
    struct position_t pos;
    dir_t dir;
};

uint8_t grid[GRID_WIDTH][GRID_HEIGHT];
char g_buttons_last;
int g_running;

FILE *driver;
int interrupt_triggered;    //TODO:: for debugging purposes only
int gamepad_init();
void grid_init(uint8_t **grid, int height, int width);
void players_init(uint8_t **grid, 
        int height, 
        int width, 
        struct player_t *players, 
        int n_players);
int update_pos(uint8_t **grid, struct player_t *players, int n_players);
int detect_crash(uint8_t **grid,int grid_heigth,int grid_length, position_t pos);

#endif
