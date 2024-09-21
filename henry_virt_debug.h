#ifndef __HENRY_VIRT_DEBUG_H__
#define __HENRY_VIRT_DEBUG_H__

#define HENRY_DBG "HENRY::->"
#define henry_virt_info(X, ...) printk(KERN_INFO HENRY_DBG X, ##__VA_ARGS__)
#define henry_virt_err(X, ...) printk(KERN_ERR HENRY_DBG X, ##__VA_ARGS__)

#endif