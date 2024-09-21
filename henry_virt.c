#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include "henry_virt_cdev.h"
#include "henry_virt_hyper.h"

#include "henry_virt_debug.h"

struct henry_virt_guest virt_guest;

static int __init
henry_virt_init(void)
{
    int rc = 0;

    henry_virt_info("Hello Henry Virtual..!");

    rc = henry_virt_hyper_init(&virt_guest);
    if(rc)
    {
        goto init_error;
    }

    if(!register_henry_virt_data())
    {
        henry_virt_info("Character Device registered");
    }
    else
    {
        rc = -EFAULT;
        goto init_error;
    }

    return 0;
init_error:
    henry_virt_err("Driver init failed [rc: %u]", rc);
    unregister_henry_virt_data();
    return rc;
}

static void __exit
henry_virt_exit(void)
{
    henry_virt_info("Bye Henry Virtual...");
    unregister_henry_virt_data();
    henry_vmx_uninit();
}

module_init(henry_virt_init);
module_exit(henry_virt_exit);