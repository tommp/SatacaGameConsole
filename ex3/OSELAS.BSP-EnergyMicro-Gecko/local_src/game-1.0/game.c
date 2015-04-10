#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "game.h"




void gpio_handler(int signo){
    //check what button has been pushed
   if(signo == SIGIO){
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
    
    if(fcntl(fileno(driver), F_GETFL) == ERROR){
        printf("ERROR: Unable to F_GETFL with the driver\n");
    }
    if(fcntl(fileno(driver), F_SETFL, oflags | FASYNC) == ERROR){
        printf("ERROR: Unable to set driver settings \n");
    }
    return 0;
}

int get_next_pos(direction_t *dir_next, position_t *head_pos){
    switch(dir_next){
        case LEFT:
            head_pos->x = head_pos.x-1
            break;
        case UP:
            head_pos->y = head_pos.y-1
            break;
        case RIGHT:
            head_pos->x = head_pos.x+1
            break;
        case DOWN:
            head_pos->y = head_pos.y+1
            break;
        default:
            printf("Error: unhandled direction\n");
    }    
}

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");

	exit(EXIT_SUCCESS);
}
