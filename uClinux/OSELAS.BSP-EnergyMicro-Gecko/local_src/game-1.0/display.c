#include <linux/fb.h>   // used for the framebuffer
#include <sys/stat.h>   // used for open()
#include <fcntl.h>      // used for open()
#include <sys/mman.h>   // used for mmap()
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>

#include "display.h"

int display_init(void){
    /* Open file reperesenting the display */
    fbfd = open("/dev/fb0", O_RDWR);
    if(fbfd == ERROR){
        printf("Error: Failed to open file\n");
        return ERROR;
        //TODO:: add exit/errno action
    }else{
        printf("Display file opened succesfully\n");
        display_fill_screen(42);
    }
    
    /* Get screen info from framebuffer driver 
    if(ioctl(fd, FBIOGET_VSCREENINFO, &screen_info) == ERROR){
        printf("Error: Failed to get screen info\n");
        //TODO:: add exit/errno action
    }else{
        printf("Screeninfo received succesfully\n");
    }
    */
    
    /* Map the screen pixel values directly to memory */
    screen_pxl_vals =(uint16_t*) mmap(
            NULL, 
            SCREEN_SIZE_BYTES, 
            PROT_READ |
            PROT_WRITE, 
            MAP_SHARED, 
            fbfd, 
            0); 
    if (screen_pxl_vals == MAP_FAILED){     //MAP_FAILED = ((void*) -1)
        printf("Error: Failed mapping screen to memory\n");
        return -1;
        //TODO:: add exit/errno action      
    }
    printf("Display initialized successfully\n");   //TODO:: Remove
    return 0;
    
}

void display_fill_screen(uint16_t value){
    int i;
    for(i=0; i<SCREEN_WIDTH * SCREEN_HEIGHT; i++){
        screen_pxl_vals[i]=value;
    }
    //update entire display
    rect.dx = 0;
    rect.dy = 0;
    rect.width = 320;
    rect.height = 240;
    printf("Trying to fill screen\n");   //TODO:: Remove
    ioctl(fbfd, 0x4680, &rect);
}

void display_fill_cell(int size, position_t pos, int color){
    for(int y= GAME_PIXEL_SIZE*pos.y; y<GAME_PIXEL_SIZE*(pos.y + 1); y++){
        for(int x= GAME_PIXEL_SIZE*pos.x; x<GAME_PIXEL_SIZE*(pos.x + 1); x++){
            screen_pxl_vals[x + y * SCREEN_WIDTH] = color;
        }
    }
    //configure cell
    rect.dx = pos.x*GAME_PIXEL_SIZE;
    rect.dy = pos.y*GAME_PIXEL_SIZE;
    rect.width = GAME_PIXEL_SIZE;
    rect.height = GAME_PIXEL_SIZE;
    
    //update configured area
    ioctl(fbfd, 0x4680, &rect);
}



