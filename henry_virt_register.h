#ifndef __HENRY_VIRT_REGISTER_H__
#define __HENRY_VIRT_REGISTER_H__

#include <linux/types.h>
#include <linux/typecheck.h>

#define IA32_VMX_BASIC 0x480

struct native_register {
    u32 eax;
    u32 ebx;
    u32 ecx;
    u32 edx;
    u64 cr4;
    union {
        u64 msr;
        struct {
            u32 msr_lower_value;
            u32 msr_upper_value;
        };
    };
};

#endif