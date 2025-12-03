#include<linux/module.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>

#define DEVICE_NAME "mychardev"

dev_t dev=MKDEV(230,0);
static struct cdev my_cdev;


static char kernal_buff[1024];
static int my_open(struct inode *inode,struct file *file)
{
    printk(KERN_INFO "mychardev:device open\n");
    printk("hi open file operation\n");
    return 0;
}

static int my_release(struct inode *inode,struct file *file)
{
    printk(KERN_INFO "mychardev:device closed\n");
    printk("bye close file operation\n");
    return 0;    
}



static ssize_t my_read(struct file *file,char __user *buf,size_t count,loff_t *f_pos)
{
    printk(KERN_INFO "read req\n");
    if(copy_to_user(buf,kernal_buff,count))
        return -EFAULT;
    return count;
}

static ssize_t my_write(struct file *file,const char __user *buf,size_t count,loff_t *f_pos)
{
    printk(KERN_INFO "write req\n");
    if(copy_from_user(kernal_buff,buf,count))
        return -EFAULT;
    return count;
}
THIS_MODULE
static struct file_operations fops={
    .owner=THIS_MODULE,
    .open=my_open,
    .read=my_read,
    .write=my_write,
    .release=my_release,
};
static int my_init(void)
{
    printk(KERN_INFO "mychardev:Initializing module\n");
    register_chrdev_region(dev,1,"bhaskar");
    cdev_init(&my_cdev,&fops);
    cdev_add(&my_cdev,dev,1);
    return 0;
}
static void e(void)
{
    unregister_chrdev_region(dev,1);
}
module_init(my_init);
module_exit(e);
MODULE_LICENSE("GPL");