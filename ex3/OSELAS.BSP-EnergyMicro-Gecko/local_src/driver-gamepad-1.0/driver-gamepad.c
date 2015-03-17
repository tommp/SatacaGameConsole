/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kdev_t.h>   // used for dev_t macros that holds the major and minor number
#include <linux/cdev.h>     // used for cdev structs (internal representation of char devices
#include <linux/fs.h>       // used for struct file_operations struct
#include <driver-gamepad.h>


/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init driver_init(void)
{
	/* Get major number for the device */
    int err = alloc_chrdev_region(&dev_nr, 0, COUNT, DEVICE_NAME);
    if(err < 0){
        printk(KERN_WARNING "Error: Unable to allocate driver region. err = %d", err);
    }else{
        printk("Driver region allocated");
    }
    
    /* init the struct for internal representation of the char driver */
    cdev_init(&driver_cdev, &fops);
    
    driver_cdev.owner = THIS_MODULE;    // TODO:: Try removing this line, it might be obsolete.
    
    /* Register the char device with the kernel */
    err = cdev_add(&driver_cdev, dev_nr, COUNT);
    if(err < 0){
        printk(KERN_WARNING "Error: Unable to register device with kernel. err = %d", err);
    }else{
        printk("Char device registered with Kernel");
    }
    
    /* Enable the driver to appear in the /dev directory */
    cl = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(cl, NULL, devno, NULL, DEVICE_NAME);
    
    
    /* Init the char device struct */	
	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void)
{
	 printk("Short life for a small module...\n");
	 
	 /* Remove the char device */
	 err = cdev_del(&driver_cdev);
    if(err < 0){
        printk(KERN_WARNING "Error: Unable to register device with kernel. err = %d", err);
    }else{
        printk("Char device registered with Kernel");
    }
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

