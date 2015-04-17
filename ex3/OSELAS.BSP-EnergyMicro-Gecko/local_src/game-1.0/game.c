#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>

#include "display.h"
#include "game.h"
#include <asm-generic/fcntl.h> // F_SETOWN

void gpio_handler(int signo){
   if(signo == SIGIO){ 
        unsigned char input = (char) getc(driver);
        update_player_direction(~input);
   }
}

void update_player_direction(char buttons_now){
    //printf("Entering update_player\n");   //TODO:: Remove
    char button_changes = buttons_now;
    printf("Button_now: %d\n", buttons_now);   //TODO:: Remove
    //printf("button_last: %d\n", g_buttons_last);   //TODO:: Remove
    //printf("button_changes: %d\n", button_changes);   //TODO:: Remove
    //Player 1 controls
    printf("dir0: %d \n", players[0].dir); 
    if(button_changes & SW_1 ) {
        printf("Switch 1 detected\n");   //TODO:: Remove
        if(players[0].dir != RIGHT) {
            players[0].dir = LEFT;
        }
        //g_running = 0; //TODO:: DEBUGGIN SHAIT, LEAVES MAIN WHILE
    }
    if(button_changes & SW_2) {
        printf("Switch 2 detected\n");   //TODO:: Remove
        if(players[0].dir != DOWN) {
        printf("SWITCH 2\n");
            players[0].dir = UP;
        }
    }
    if(button_changes & SW_3) {
        printf("Switch 3 detected\n");   //TODO:: Remove
        if(players[0].dir != LEFT) {
            players[0].dir = RIGHT;
            printf("dir0: %d \n", players[0].dir); 
        }
        printf("dir1: %d \n", players[0].dir); 
    }
    if(button_changes & SW_4) {
        printf("Switch 4 detected\n");   //TODO:: Remove
        if(players[0].dir != UP) {
            players[0].dir = DOWN;
        }
    }
    //Player 2 controls
    if(button_changes & SW_5) {
        printf("Switch 5 detected\n");   //TODO:: Remove
        if(players[1].dir != RIGHT) {
            players[1].dir = LEFT;
        }
    }
    if(button_changes & SW_6) {
        printf("Switch 6 detected\n");   //TODO:: Remove
        if(players[1].dir != DOWN) {
            players[1].dir = UP;
        }
    }
    if(button_changes & SW_7) {
        printf("Switch 7 detected\n");   //TODO:: Remove
        if(players[1].dir != LEFT) {
            players[1].dir = RIGHT;
        }
    }
    if(button_changes & SW_8) {
        printf("Switch 8 detected\n");   //TODO:: Remove
        if(players[1].dir != UP) {
            players[1].dir = DOWN;
        }
    }
    g_buttons_last = buttons_now;
    printf("dir2: %d \n", players[0].dir);   //TODO:: Remove
} 

int gamepad_init(void){

    printf("Initializing gamepad \n");
    
    driver = fopen("/dev/driver-gamepad", "rb");
    if(driver == NULL){
        printf("ERROR: Unable to open driver-gamepad file\n");
        exit(1);
    }

    if(signal(SIGIO, &gpio_handler) == SIG_ERR){
        printf("ERROR: Unable to register signal handler callback\n");
        exit(1);
    }
    if(fcntl(fileno(driver), F_SETOWN, getpid()) == ERROR){    
        printf("ERROR: Unable to register process with driver\n");
        exit(1);
    }
    int oflags = fcntl(fileno(driver), F_GETFL);
    if(oflags == ERROR){
        printf("ERROR: Unable to F_GETFL with the driver\n");
        exit(1);
    }
    if(fcntl(fileno(driver), F_SETFL, oflags | FASYNC) == ERROR){
        printf("ERROR: Unable to set driver settings \n");
        exit(1);
    }
    printf("Exiting gamepad_init\n");   //TODO:: Remove
    return 0;
}

// Initialize all grid cells to zero
void grid_init(uint8_t grid[][GRID_HEIGHT]){
    printf("Entering grid_init\n");   //TODO:: Remove
    for (int row = 0; row < GRID_HEIGHT; row++){
        for (int col = 0; col < GRID_WIDTH; col++){
            grid[col][row] = CELL_FREE;
        }
    }
    printf("Exiting grid_init\n");   //TODO:: Remove
}

void players_init(uint8_t grid[][GRID_HEIGHT], player_t *players){
    printf("Entering players_init\n");   //TODO:: Remove
    for (int player = 0; player < N_PLAYERS; player++){
        players[player].pos.x = GRID_WIDTH*(player+1)/(N_PLAYERS+1);
        players[player].pos.y = GRID_HEIGHT/2;
        
        players[player].dir = UP;
    }
        printf("Exiting players_init\n");   //TODO:: Remove
}

int update_pos(uint8_t grid[][GRID_HEIGHT], player_t players[N_PLAYERS]){
    //printf("Entering update_pos\n");   //TODO:: Remove
    for(int player=0; player < N_PLAYERS; player++){
        switch(players[player].dir){
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
                printf("Error: unhandled direction\n");
                return -1;
        }
        if(detect_crash(grid, players[player].pos)){
            //printf("Detected crash\n");   //TODO:: Remove
            //handle death
            g_running = 0;
            display_fill_screen((player)*0x7FF0);
            return 1;
        }else{
            grid[players[player].pos.x][players[player].pos.y] = (player+1);
            display_fill_cell(GAME_PIXEL_SIZE, players[player].pos, (player+1)*0x7FF0);
        }
    }
    //printf("Exiting update_pos\n");   //TODO:: Remove
    return 0;
}

// returns 1 if move is illigal, 0 otherwise
int detect_crash(uint8_t grid[][GRID_HEIGHT], position_t pos){
    //Check if position is inside the grid bounderies
    if ((pos.x>GRID_WIDTH-1) || (pos.x<0)){
        //out of bounds sideways
        return 1;
    }
    if ((pos.y>GRID_HEIGHT-1) || (pos.y<0)){
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
	
	display_init();
    gamepad_init();
    grid_init(grid);
    players_init(grid, players);
    
    display_fill_screen(0);
    g_running = 1;
    
    while(g_running) {
        start = clock();
        
        update_pos(grid, players);
        
        end = clock();
        
        remaining_time = TIME_PER_LOOP - (((end - start)/CLOCKS_PER_SEC)*1000);
        
        if(remaining_time > 0) {
            usleep(remaining_time);
        }
        
        continue;
    }
	exit(EXIT_SUCCESS);
}
