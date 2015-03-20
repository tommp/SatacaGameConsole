#ifndef _DISPLAY_API_H
#define _DISPLAY_API_H

#define DISPLAY_HEIGHT 240
#define DISPLAY_WIDTH 320

/* Struct to hold the rectangle of the framebuffer that is to be updated */
struct fb_copyarea rect;

/* file descriptor for det display buffer */
int fd;




#endif // _DISPLAY_API_H
