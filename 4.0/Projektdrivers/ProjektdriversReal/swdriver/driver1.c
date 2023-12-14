#include <linux/gpio.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>

static int gpio_pin = 16;
const int first_minor = 0;
const int max_devices = 255;
static dev_t devno;
static struct class *sw2_class;
static struct cdev sw2_cdev;
int sw2_open(struct inode *inode, struct file *filep)
{
    int major, minor;
    major = MAJOR(inode->i_rdev);
    minor = MINOR(inode->i_rdev);
    printk("Opening SW2 Device [major], [minor]: %i, %i\n", major, minor);
    
    return 0;
}
int sw2_release(struct inode *inode, struct file *filep)
{
    int minor, major;
    major = MAJOR(inode->i_rdev);
    minor = MINOR(inode->i_rdev);
    printk("Closing/Releasing SW2 Device [major], [minor]: %i, %i\n", major, minor);
    return 0;
}
ssize_t sw2_read(struct file *filep, char __user *ubuf, size_t count, loff_t *f_pos) {
    char kbuf[12];
    int len, value;
    value = gpio_get_value(gpio_pin);
    len = count < 12 ? count : 12;
    len = snprintf(kbuf, len, "%i", value);
    long err = copy_to_user(ubuf, kbuf, ++len);
    if (err < 0) {
        pr_err("SW2: ERROR failed to copy to user\n");
        return err;
    }
    *f_pos += len;
    return len;
}

struct file_operations sw2_fops = {
    .owner      = THIS_MODULE,
    .open       = sw2_open,
    .release    = sw2_release,
    .read       = sw2_read
};
static int __init sw2_init(void)
{
    int err = 0;
    pr_info("SW2: Initializing\n");
    err = gpio_request(gpio_pin, "sw2");
    if (err < 0) {
        pr_err("SW2: ERROR on request GPIO\n");
        goto fail_req;
    }
    err = gpio_direction_input(gpio_pin);
    if (err < 0) {
        pr_err("SW2: ERROR on settitng direction\n");
        goto fail_dir;
    }
    err = alloc_chrdev_region(&devno, first_minor, max_devices, "sw2-driver");
    if (MAJOR(devno) <= 0) {
        pr_err("SW2: ERROR failed to register chardev\n");
        goto fail_alloc;
    }
    pr_info("SW2: Driver got Major %i\n", MAJOR(devno));
    sw2_class = class_create(THIS_MODULE, "sw2-class");
    if (IS_ERR(sw2_class)) {
        pr_err("SW2: ERROR Failed to create class\n");
        goto fail_class;
    }
    cdev_init(&sw2_cdev, &sw2_fops);
    err = cdev_add(&sw2_cdev, devno, 255);
    if (err) {
        pr_err("SW2: ERROR failed to add cdev\n");
        goto fail_cdev;
    }
    pr_info("SW2: Initialized succes\n");
    return err;
    fail_cdev:  class_destroy(sw2_class);
    fail_class: unregister_chrdev_region(devno, max_devices);
    fail_alloc: 
    fail_dir:   gpio_free(gpio_pin);
    fail_req:
    return err;
}
static void sw2_exit(void)
{
    pr_info("SW2: Exiting\n");
    cdev_del(&sw2_cdev);
    class_destroy(sw2_class);
    unregister_chrdev_region(devno, max_devices);
    gpio_free(gpio_pin);
    pr_info("SW2: Exit succes\n");
}
module_init(sw2_init);
module_exit(sw2_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Halkanhalkanikke");
MODULE_DESCRIPTION("Driver til switch2");