#ifndef _GAME_H
#define _GAME_H

//TODO:: THESE ARE DOUBLEDEFINES!!!! REMOVE!
#define SCREEN_HEIGHT   240                             //
#define SCREEN_WIDTH    320                             //
#define GAME_PIXEL_SIZE 4                               //
#define GRID_HEIGHT     SCREEN_HEIGHT/GAME_PIXEL_SIZE   //
#define GRID_WIDTH      SCREEN_WIDTH/GAME_PIXEL_SIZE    //

#define FASYNC      0x0040                              //
#define F_SETOWN    6                                   //


#define CELL_FREE       0
#define CELL_BLOCKED    1

#define TIME_PER_LOOP   100
#define N_PLAYERS       2

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

typedef struct position{
    uint32_t x;
    uint32_t y;
}position_t;

typedef struct player{
    position_t pos;
    dir_t dir;
}player_t;

char g_buttons_last;
int g_running;
player_t players[N_PLAYERS];

FILE *driver;
int interrupt_triggered;    //TODO:: for debugging purposes only
int detect_crash(uint8_t grid[][GRID_HEIGHT], position_t pos);
int gamepad_init(void);
void grid_init(uint8_t grid[][GRID_HEIGHT]);
void players_init(uint8_t grid[][GRID_HEIGHT], player_t players[N_PLAYERS]);
int update_pos(uint8_t grid[][GRID_HEIGHT], player_t players[N_PLAYERS]);
void update_player_direction(player_t players[N_PLAYERS], char buttons_now);

#endif
