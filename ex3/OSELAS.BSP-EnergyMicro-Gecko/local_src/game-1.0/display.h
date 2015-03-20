#ifndef _DISPLAY_H
#define _DISPLAY_H

#define SCREEN_HEIGHT   240
#define SCREEN_WIDTH    320
#define BITS_PER_PIXEL  16
#define SCREEN_SIZE_BYTES (SCREEN_HEIGHT*SCREEN_WIDTH*BITS_PER_PIXEL/8)
#define N_BITS_RED      5
#define N_BITS_GREEN    5
#define N_BITS_BLUE     6

#define ERROR   (-1)

/* Struct to hold the rectangle of the framebuffer that is to be updated */
struct fb_copyarea rect;

/* file descriptor for det display buffer */
int fbfd;

/* struct fb_var_screeninfo stores device independent changeable information
about a frame buffer device, its current format and video mode, as well as
other miscellaneous parameters */
struct fb_var_screeninfo screen_info;


uint16_t *screen_pxl_vals;

int display_init(void);

#endif // _DISPLAY_API_H
