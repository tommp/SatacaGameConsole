
#define DEVICE_NAME "driver-gamepad"
#define COUNT    1
#define GPIO_IN_EN  0x33333333
#define GPIO_PULL_DIR_UP 0xff

static int __init driver_init(void);
static void __exit driver_cleanup(void);
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

/* Struct to hold the device numbers */
dev_t dev_nr;

/* The kernel's internal representation of char devices */
struct cdev driver_cdev;

/* Pointer tp point to to GPIO memory if it is awailable */
struct resource *resource;

struct class *cl;
