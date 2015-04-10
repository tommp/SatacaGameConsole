#include <linux/fb.h>   // used for the framebuffer
#include <sys/stat.h>   // used for open()
#include <fcntl.h>      // used for open()
#include <sys/mman.h>   // used for mmap()

#include "display.h"


int display_init(void){

    /* Open file reperesenting the display */
    fbfd = open("/dev/fb0", O_RDWR);
    if(fbfd == ERROR){
        printf("Error: Failed to open file\n");
        //TODO:: add exit/errno action
    }else{
        printf("Display file opened succesfully\n");
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
            fp, 
            0); 
    if (screen_pxl_vals == MAP_FAILED){     //MAP_FAILED = ((void*) -1)
        printf("Error: Failed mapping screen to memory\n");
        //TODO:: add exit/errno action      
    }
    
}

void fill_screen(uint16_t value){
    for(int i=0; i<SCREEN_WIDTH * SCREEN_HEIGHT; i++){
        screen_pxl_vals[i]=value;
    }
    //comand driver to update display
    rect.dx = 0;
    rect.dy = 0;
    rect.width = 320;
    rect.height = 240;
    ioctl(fp, 0x4680, &rect);
}

/*
void update_snakes(uint16_t* screen_pxl_vals, struct snake_t *players, n_players){
    // Update each player 
    for(int player=0; player<n_players; player++){ 
        //update move to mmap buffer
        
        //update only head on screen
    }
    //TODO:: this is only for debugging, replace with smarter refreshing.
    rect.dx = 0;
    rect.dy = 0;
    rect.width = 320;
    rect.height = 240;
    ioctl(fp, 0x4680, &rect);
}
*/


