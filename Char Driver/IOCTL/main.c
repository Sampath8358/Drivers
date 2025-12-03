#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define MY_DEVFILE_NAME "myDevFile"
#define MY_DRIVER_NAME  "myCharDriver"

#define IOCTL_READ  _IOR('d', 1, char)
#define IOCTL_WRITE _IOW('d', 2, char)

struct cdev my_dev;
dev_t dev_number;
static char kernal_buff[100];
static struct class *my_class;

static int my_open(struct inode *inode, struct file *file)
{
    pr_info("%s file opened\n", MY_DEVFILE_NAME);
    return 0;
}
static int my_release(struct inode *inode, struct file *file)
{
    pr_info("%s file closed\n", MY_DEVFILE_NAME);
    return 0;
}
// static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
// {
//     if (copy_to_user(buf, kernal_buff, len))
//         return -EFAULT;
//     return len;
// }
// static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
// {
//     if (copy_from_user(kernal_buff, buf, len))
//         return -EFAULT;
//     return len;
// }
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
        case IOCTL_READ:
            if (copy_to_user(arg,kernal_buff, sizeof(kernal_buff)))
                return -EFAULT;
            pr_info("IOCTL_READ: Read data from kernel buffer\n");
            break;

        case IOCTL_WRITE:
            if (copy_from_user(kernal_buff,arg, sizeof(kernal_buff)))
                return -EFAULT;
            pr_info("IOCTL_WRITE: Wrote data to kernel buffer\n");
            break;

        default:
            return -EINVAL; 
    }
    return 0;
}
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    // .read = my_read,
    // .write = my_write,
    .release = my_release,
    .unlocked_ioctl = my_ioctl, 
};

static int my_init(void)
{
    int ret = alloc_chrdev_region(&dev_number, 0, 1, MY_DRIVER_NAME);
    
    cdev_init(&my_dev, &fops);
    cdev_add(&my_dev, dev_number, 1);
    my_class = class_create("me_class");
    device_create(my_class, NULL, dev_number, NULL, MY_DEVFILE_NAME);
    pr_info("ret = %d\n",ret);
    return 0;
}

static void my_exit(void)
{
    cdev_del(&my_dev);
    unregister_chrdev_region(dev_number, 1);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");