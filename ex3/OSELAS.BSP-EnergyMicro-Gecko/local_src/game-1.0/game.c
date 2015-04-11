#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "display.h"
#include "game.h"

void gpio_handler(int signo){
   if(signo == SIGIO){ 
        fill_screen(6);
        update_player_direction(&players, (char) getc(driver));
   }
}

int update_player_direction(struct player_t *players, char buttons_now){

    char button_changes = (buttons_now ^ g_buttons_last);
    
    //Player 1 controls
    if(button_changes & SW_1) {
        if(players[0].dir != RIGHT) {
            players[0].dir = LEFT;
        }
        g_running = 0; //TODO:: DEBUGGIN SHAIT, LEAVES MAIN WHILE
    }
    if(button_changes & SW_2) {
        if(players[0].dir != DOWN) {
            players[0].dir = UP;
        }
    }
    if(button_changes & SW_3) {
        if(players[0].dir != LEFT) {
            players[0].dir = RIGHT;
        }
    }
    if(button_changes & SW_4) {
        if(players[0].dir != UP) {
            players[0].dir = DOWN;
        }
    }
    //Player 2 controls
    if(button_changes & SW_5) {
        if(players[1].dir != RIGHT) {
            players[1].dir = LEFT;
        }
    }
    if(button_changes & SW_6) {
        if(players[1].dir != DOWN) {
            players[1].dir = UP;
        }
    }
    if(button_changes & SW_7) {
        if(players[1].dir != LEFT) {
            players[1].dir = RIGHT;
        }
    }
    if(button_changes & SW_8) {
        if(players[1].dir != UP) {
            players[1].dir = DOWN;
        }
    }
    g_buttons_last = buttons_now;
} 

int gamepad_init(){

    printf("Initializing gamepad \n");
    
    driver = fopen("/dev/gamepad", "rb");

    if(signal(SIGIO, &gpio_handler) == SIG_ERR){
        printf("ERROR: Unable to register signal handler callback\n");
    }
    if(fcntl(fileno(driver), F_SETOWN, getpid()) == ERROR){
        printf("ERROR: Unable to register process with driver\n");
    }
    int oflags = fcntl(fileno(driver), F_GETFL);
    if(oflags == ERROR){
        printf("ERROR: Unable to F_GETFL with the driver\n");
    }
    if(fcntl(fileno(driver), F_SETFL, oflags | FASYNC) == ERROR){
        printf("ERROR: Unable to set driver settings \n");
    }
    return 0;
}

// Initialize all grid cells to zero
void grid_init(uint8_t **grid, int height, int width){
    for (int row = 0; row < height; row++){
        for (int col = 0; col < width; col++){
            grid[col][row] = CELL_FREE;
        }
    }
}

void players_init(uint8_t **grid, int height, int width, struct player_t *players, int n_players){
    for (int player = 0; player < n_players; player++){
        players[player].pos.x = width*(player+1)/(n_players+1);
        players[player].pos.y = height/2;
        
        players[player].dir_next = UP;
    }
}

int update_pos(uint8_t **grid, struct player_t *players, int n_players){
    for(int player=0; player < n_players; player++){
        switch(players[player].dir_next){
            case LEFT:
                players[player].pos.x = players[player].pos.x-1;
                break;
            case UP:
                players[player].pos.y = players[player].pos.y-1;
                break;
            case RIGHT:
                players[player].pos.x = players[player].pos.x+1;
                break;
            case DOWN:
                players[player].pos.y = players[player].pos.y+1;
                break;
            default:
                return -1;
                printf("Error: unhandled direction\n");
        }
        if(detect_crash(grid, GRID_HEIGHT, GRID_WIDTH, players[player].pos){
            //handle death
            return 1;
        }else{
            grid[players[player].pos.x][players[player].pos.y] = (player+1);
            display_fill_cell(GAME_PIXEL_SIZE, players[player].pos, (player+1));
        }
    }
    return 0;
}

// returns 1 if move is illigal, 0 otherwise
int detect_crash(uint8_t **grid,int grid_heigth, int grid_length, position_t pos){
    //Check if position is inside the grid bounderies
    if ((pos.x>grid_length-1) || (pos.x<0)){
        //out of bounds sideways
        return 1;
    }
    if ((pos.y>grid_height-1) || (pos.y<0)){
        //out of bounds in the other direction
        return 1;
    }
    
    if(grid[pos.x][pos.y] > 0){
        return 1;
    }else{
        return 0;
    }
}


int main(int argc, char *argv[])
{
	printf("Initializing game\n");
	clock_t start, end;
	int remaining_time;
	struct player_t players[NUM_PLAYERS];
	
	display_init();
    gamepad_init();
    grid_init(grid, GRID_HEIGHT, GRID_WIDTH);
    players_init(grid, GRID_HEIGHT, GRID_WIDTH, players, NUM_PLAYERS);
    
    fill_screen(13);
    g_running = 1;
    
    while(running) {
        start = clock();
        
        update_position(grid, players, NUM_PLAYERS);
        
        end = clock();
        
        remaining_time = TIME_PER_LOOP - (((end - start)/CLOCKS_PER_SEC)*1000);
        
        if(remaining_time > 0) {
            usleep(remaining_time*1000);
        }
        
        continue;
    }
	exit(EXIT_SUCCESS);
}
