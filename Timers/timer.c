#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/timer.h>

#define DEVICE_NAME "mychardev"
void timer_fun(struct timer_list *my_timer);
int count = 0;
dev_t dev_number;
struct cdev my_cdev;
struct class *my_class;
struct timer_list my_timer;
static int ker_buf = 0;
static int my_open(struct inode *inode, struct file *file)
{
    pr_info("%s file is opened\n", DEVICE_NAME);
    return 0;
}
static int my_release(struct inode *inode, struct file *file)
{
    pr_info("%s file is closed\n", DEVICE_NAME);
    return 0;
}
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
    pr_info("reading....\n");
    return len;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
    pr_info("writing....\n");
    int user_buf;
    copy_from_user(&ker_buf,&user_buf,sizeof(ker_buf));
        
    return len;
}
void timer_fun(struct timer_list *my_timer)
{
    count++;
    pr_info("In timer function count = %d\n",count);
    mod_timer(my_timer, jiffies + msecs_to_jiffies(1000));
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

static int my_init(void)
{
    alloc_chrdev_region(&dev_number, 0, 1, DEVICE_NAME);
    cdev_init(&my_cdev, &fops);
    cdev_add(&my_cdev, dev_number, 1);
    my_class = class_create("my_class");
    device_create(my_class, NULL, dev_number, NULL, DEVICE_NAME);
    timer_setup(&my_timer, timer_fun, 0);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
    return 0;
}

static void my_exit(void)
{
    device_destroy(my_class, dev_number);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_number, 1);
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
