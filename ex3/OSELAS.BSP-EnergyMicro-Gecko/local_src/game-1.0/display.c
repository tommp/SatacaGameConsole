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

