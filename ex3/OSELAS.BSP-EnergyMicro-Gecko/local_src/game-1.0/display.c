#include <linux/fb.h>   // used for the framebuffer
#include <sys/stat.h>   // used for open()
#include <fcntl.h>      // used for open()

#include "display.h"


int display_init(){
    fd = open("/dev/fb0", O_RDWR);
    if(fd == -1){
           
    }
    
    
}

