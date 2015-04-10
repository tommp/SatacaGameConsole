#ifndef _DRIVER_GAMEPAD_H 
#define _DRIVER_GAMEPAD_H

#define DEVICE_NAME "driver-gamepad"
#define COUNT    1
#define GPIO_IN_EN  0x33333333
#define GPIO_PULL_DIR_UP 0xff
#define GPIO_EVEN_IRQ_NUM 17
#define GPIO_ODD_IRQ_NUM 18

static int __init init_driver();
static void __exit driver_cleanup();
static int driver_open(struct inode *node, struct file *filp);
static int driver_release(struct inode *inode, struct file *filp);
static ssize_t driver_read(
        struct file *filp, 
        char __user *buff, 
        size_t count, 
        loff_t *offp);
static ssize_t driver_write(
        struct file *filp, 
        const char __user *buf, 
        size_t count, 
        loff_t *offp);
static irqreturn_t GPIO_interrupt_handler(int irq, void *dev_id, struct pt_regs * regs);
static int driver_fasync(int fd, struct file *filp, int mode);

/* Struct to hold the device numbers */
dev_t dev_nr;

/* The kernel's internal representation of char devices */
struct cdev driver_cdev;

/* Pointer tp point to to GPIO memory if it is awailable */
struct resource *resource;

struct class *cl;

/* A stuct representing the asynchronus queue for signal handling */
struct fasync_struct *async_queue;

uint32_t driver_enabled; 

#endif
