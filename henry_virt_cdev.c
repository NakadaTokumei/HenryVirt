#include <linux/kernel.h>

#include "henry_virt_cdev.h"
#include "henry_virt_debug.h"

static int
henry_virt_dev_open(
    struct inode* node,
    struct file* file
)
{
    struct henry_virt_data* virt_data;

    virt_data = container_of(node->i_cdev, struct henry_virt_data, char_dev);

    file->private_data = virt_data;
    
    return 0;
}

static int
henry_virt_dev_release(
    struct inode* node,
    struct file* file
)
{
    struct henry_virt_data* virt_data;

    virt_data = (struct henry_virt_data*)file->private_data;

    return 0;
}

static ssize_t
henry_virt_dev_read(
    struct file* file,
    char __user* user_data,
    size_t len,
    loff_t* loff
)
{
    struct henry_virt_data* virt_data;

    henry_virt_info("Read operation occured\n");

    virt_data = (struct henry_virt_data*)file->private_data;

    return len;
}

static ssize_t
henry_virt_dev_write(
    struct file* file,
    const char __user* user_data,
    size_t len,
    loff_t* loff
)
{
    struct henry_virt_data* virt_data;

    henry_virt_info("Write operation occured\n");

    virt_data = (struct henry_virt_data*)file->private_data;

    return len;
}

struct file_operations henry_virt_fops = {
    .owner = THIS_MODULE,
    .open = henry_virt_dev_open,
    .release = henry_virt_dev_release,
    .read = henry_virt_dev_read,
    .write = henry_virt_dev_write,
};

int
register_henry_virt_data(void)
{
    int err;

    henry_virt_info("%s entered...\n", __func__);
    err = register_chrdev(
        HENRY_VIRT_MAJOR, 
        HENRY_VIRT_NAME,
        &henry_virt_fops 
    );
    if(err)
    {
        henry_virt_err("register_chrdev failed...\n");
        goto register_err;
    }

    return 0;
register_err:
    henry_virt_err("%s error: [%d]\n", __func__, err);
    unregister_henry_virt_data();
    return err;
}

void
unregister_henry_virt_data(void)
{
    henry_virt_info("%s entered...", __func__);

    unregister_chrdev(HENRY_VIRT_MAJOR, HENRY_VIRT_NAME);
}