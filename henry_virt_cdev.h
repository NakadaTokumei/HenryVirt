#ifndef __HENRY_VIRT_CDEV_H__
#define __HENRY_VIRT_CDEV_H__

#include <linux/fs.h>
#include <linux/cdev.h>

#define HENRY_VIRT_MAJOR 45
#define HENRY_VIRT_NAME "henry_virt"

struct henry_virt_data {
    struct cdev char_dev;   // Char Device
};

int 
register_henry_virt_data(void);

void
unregister_henry_virt_data(void);

#endif
