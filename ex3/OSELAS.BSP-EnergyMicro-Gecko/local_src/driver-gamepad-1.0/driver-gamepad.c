/*
 * This is a demo Linux kernel module.
 */
 
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kdev_t.h>   // used for dev_t macros that holds the major and minor number
#include <linux/cdev.h>     // used for cdev structs (internal representation of char devices
#include <linux/fs.h>       // used for struct file_operations struct
#include <asm/io.h>         // used for iowrite32 and other io operations
#include <linux/ioport.h>   // used for request_mem_region() and other
//#include <sys/types.h>      // used for ssize_t
#include <linux/signal.h>   //Used to send signals
#include <asm/uaccess.h>    // used for copy_to_user
#include <linux/irq.h>
#include <linux/device.h>

#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <asm/signal.h>
#include <linux/poll.h>
#include <asm/siginfo.h>
#include <linux/moduleparam.h>

#include "efm32gg.h"
#include "driver-gamepad.h"

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init init_driver()
{
	/* Get major number for the device */
	int err = 0;
    err = alloc_chrdev_region(&dev_nr, 0, COUNT, DEVICE_NAME);
    if(err < 0){
        printk(KERN_WARNING "Error: Unable to allocate driver region. err = %d\n", err);
    }else{
        printk("Driver region allocated\n");
    }
    /* init struct to register the different services offered by the driver */
    static struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = driver_read,
        .write = driver_write,
        .open = driver_open,
        .release = driver_release,
        .fasync = driver_fasync,
    };
    
    /* init the struct for internal representation of the char driver */
    cdev_init(&driver_cdev, &fops);
    
    driver_cdev.owner = THIS_MODULE;    // TODO:: Try removing this line, it might be obsolete.
    
    /* Register the char device with the kernel */
    err = cdev_add(&driver_cdev, dev_nr, COUNT);
    if(err < 0){
        printk(KERN_WARNING "Error: Unable to register device with kernel. err = %d\n", err);
    }else{
        printk("Char device registered with Kernel\n");
    }
    
    /* Enable the driver to appear in the /dev directory */
    cl = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(cl, NULL, dev_nr, NULL, DEVICE_NAME);
    
    /* Request access to GPIO memory region*/
    resource = request_mem_region(GPIO_PC_BASE, GPIO_IFC_OFFSET, DEVICE_NAME);
    if(resource == NULL){
        printk(KERN_WARNING "Error: GPIO memory access denied\n");
    }else{
        printk("Access to memory region granted\n");
    }
    
    /*Configure buttons as input*/
    iowrite32(GPIO_IN_EN, GPIO_PC_MODEL);
    iowrite32(GPIO_PULL_DIR_UP, GPIO_PC_DOUT);

    /* Configure hardware interrupt */
    iowrite32(0x22222222, GPIO_EXTIPSELL);
    iowrite32(0xFF, GPIO_EXTIFALL);
    iowrite32(0xFF, GPIO_IEN);

    /* Register interrupt handler */
    request_irq(GPIO_EVEN_IRQ_NUM,(irq_handler_t)GPIO_interrupt_handler, 0, DEVICE_NAME, &driver_cdev);
    request_irq(GPIO_ODD_IRQ_NUM, (irq_handler_t)GPIO_interrupt_handler, 0, DEVICE_NAME, &driver_cdev);  
    	
	return 0;
}

static int driver_open(struct inode *node, struct file *filp){
    printk("More work?\n");
    driver_enabled = 1;
    return 0;
}

/*User program close the driver*/
static int driver_release(struct inode *inode, struct file *filp){
    printk("Jobs done!\n");
    driver_enabled = 0;
    return 0;
}

/*Function that is run when user tries to read from the device */
static ssize_t driver_read(
        struct file *filp, 
        char __user *buff, 
        size_t count, 
        loff_t *offp){
        
    uint32_t data = ioread32(GPIO_PC_DIN);
    copy_to_user(buff, &data, 1);
    return 1;
}

static ssize_t driver_write(
        struct file *filp, 
        const char __user *buf, 
        size_t count, 
        loff_t *offp){
    return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit driver_cleanup()
{
	 printk("Short life for a small module...\n");
	 
	/* Remove the char device */
	cdev_del(&driver_cdev);

    /* Release interrupt handler */
    free_irq(GPIO_EVEN_IRQ_NUM, &driver_cdev);
    free_irq(GPIO_ODD_IRQ_NUM, &driver_cdev);
    
    /* Release occupied memory so that it can be used by other drivers */
    release_mem_region(GPIO_PC_BASE, GPIO_IFC_OFFSET);
}

static int driver_fasync(int fd, struct file *filp, int mode){
    return fasync_helper(fd, filp, mode, &async_queue); 
}

static irqreturn_t GPIO_interrupt_handler(int irq, void *dev_id, struct pt_regs * regs)
{

    /* Resets the interrupt */
    iowrite32(0xFFFF, GPIO_IFC);

    /* If there are any prosesses in the queue, send the signal with kill fasynch */
    if(async_queue){
        kill_fasync(&async_queue, SIGIO, POLL_IN);
    }
    
    /* Use this in external files to recieve signals from the driver
    signal(SIGIO, &input_handler); input handler is the function to be called when a signal is recieved
    fcntl(STDIN_FILENO, F_SETOWN, getpid());
    oflags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, oflags | FASYNC);
    */

    return IRQ_HANDLED; 
}

module_init(init_driver);
module_exit(driver_cleanup);

MODULE_DESCRIPTION("Small driver for the gamepad, super useful");
MODULE_LICENSE("GPL");

