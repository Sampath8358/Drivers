#include <linux/init.h>
#include <linux/module.h>
#include <linux/wait.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
MODULE_LICENSE("GPL");
int my_open(struct inode *inode, struct file *file);
int my_release(struct inode *inode, struct file *file);
ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *ppos);
ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos);
DECLARE_WAIT_QUEUE_HEAD(wq);
//static char mbuff[50];
int flag = 0;
static dev_t wdev = MKDEV(300, 0);
static struct cdev wcdev;

int my_open(struct inode *inode, struct file *file)
{
    pr_info("file opened\n");
    return 0;
}
int my_release(struct inode *inode, struct file *file)
{
    pr_info("file closed\n");
    return 0;
}
ssize_t read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
    //copy_to_user(mbuff, buf, sizeof(mbuff));
    pr_info("read:read done\n");
    pr_info("going to sleep\n");
    wait_event_interruptiable(wq, 1 == flag);
    pr_info("came out of sleep\n");
    return len;
}
ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
    //copy_from_user(buf, mbuff, sizeof(mbuff));
    //pr_info("%s\n", mbuff);
    pr_info("write done\n");
    flag = 1;
    wake_up(&wq);
    pr_info("flag changed\n");
    return len;
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
    register_chrdev_region(wdev, 1, "wadriver");
    cdev_init(&wcdev, &fops);
    cdev_add(&wcdev, wdev, 1);
    return 0;
}
static void my_exit(void)
{
    unregister_chrdev_region(wdev, 1);
    pr_info("exiting the module\n");
}
module_init(my_init);
module_exit(my_exit);
