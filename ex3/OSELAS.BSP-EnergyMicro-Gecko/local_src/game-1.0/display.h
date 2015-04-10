#ifndef _DISPLAY_API_H
#define _DISPLAY_API_H

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

#define SCREEN_HEIGHT   240
#define SCREEN_WIDTH    320
#define BITS_PER_PIXEL  16
#define SCREEN_SIZE_BYTES (SCREEN_HEIGHT*SCREEN_WIDTH*BITS_PER_PIXEL/8)
#define N_BITS_RED      5
#define N_BITS_GREEN    5
#define N_BITS_BLUE     6
#define GAME_PIXEL_SIZE 4

#define ERROR   (-1)

/* file descriptor for det display buffer */
int fbfd;

/* Struct to hold the rectangle of the framebuffer that is to be updated */
struct fb_copyarea* rect;

/* struct fb_var_screeninfo stores device independent changeable information
about a frame buffer device, its current format and video mode, as well as
other miscellaneous parameters */
struct fb_var_screeninfo* screen_info;

uint16_t *screen_pxl_vals;

int display_init();
void fill_screen(uint16_t value);
void display_fill_cell(int size, struct position_t pos, int color)

#endif // _DISPLAY_API_H
