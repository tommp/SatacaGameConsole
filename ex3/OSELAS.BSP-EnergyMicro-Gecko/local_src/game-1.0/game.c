#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>

#include "display.h"
#include "game.h"

void gpio_handler(int signo){
   if(signo == SIGIO){
        //TODO:: for debugging purposes only
        interrupt_triggered = 1;    
        fill_screen(6);
   }
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
void grid_init(bool *grid, int height, int width){
    for (int row = 0; row < height; row++){
        for (int col = 0; col < width; col++){
            grid[row][col] = 0;
        }
    }
}

void players_init(bool *grid, int height, int width, struct player_t *players, int n_players){
    for (int player = 0; player < n_players; player++){
        players[player].pos.x = width*(player+1)/(n_players+1);
        players[player].pos.y = height/2;
        
        players[player].dir_next = UP;
    }
}

int update_pos(dir_t *dir_next, position_t *head_pos){
    switch(*dir_next){
        case LEFT:
            head_pos->x = head_pos->x-1;
            break;
        case UP:
            head_pos->y = head_pos->y-1;
            break;
        case RIGHT:
            head_pos->x = head_pos->x+1;
            break;
        case DOWN:
            head_pos->y = head_pos->y+1;
            break;
        default:
            return -1;
            printf("Error: unhandled direction\n");
    }
    return 0;
}

int main(int argc, char *argv[])
{
	printf("Initializing game\n");
	interrupt_triggered = 0;        //TODO:: for debugging purposes only
	display_init();
	fill_screen(13);
    gamepad_init();
    while(running) {
        continue;
    }

	exit(EXIT_SUCCESS);
}
