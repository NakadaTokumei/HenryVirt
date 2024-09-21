#ifndef __HENRY_VIRT_ASM_H__
#define __HENRY_VIRT_ASM_H__

#include "henry_virt_register.h"

static inline void
__henry_cpuid(
    u32 eax,
    struct native_register* reg
)
{
    asm volatile("cpuid"
                : "=a"(reg->eax), 
                  "=b"(reg->ebx),
                  "=c"(reg->ecx),
                  "=d"(reg->edx)
                : "a"(eax)
                : "memory");
}

static inline void
__henry_read_msr(
    u32 ecx,
    struct native_register* reg
)
{
    asm volatile("rdmsr"
                : "=a"(reg->msr_lower_value),
                  "=d"(reg->msr_upper_value)
                : "c"(ecx)
                : "memory");
}

static inline void
__henry_get_cr4(
    struct native_register* reg
)
{
    asm volatile("mov %%cr4, %0"
                 : "=r"(reg->cr4)
                 : 
                 : "memory");
}

static inline void
__henry_set_cr4(
    u64 rax
)
{
    asm volatile("mov %0, %%cr4"
                 :
                 : "r"(rax)
                 : "memory");
}

static inline int
__henry_vmxon(
    void* mem
)
{
    uint8_t ret;
    asm volatile("vmxon %[pm]\n" 
                 "setna %[ret]"
                 : [ret]"=rm"(ret)
                 : [pm]"m"(mem)
                 : "cc", "memory");
    return ret;
}

static inline void
__henry_vmxoff(void)
{
    asm volatile("vmxoff":::"cc", "memory");
}

static inline int
__henry_vmptrld(
    void* mem
)
{
    uint8_t ret;
    asm volatile("vmptrld %[pm]\n"
                 "setna %[ret]"
                 : [ret]"=rm"(ret)
                 : [pm]"m"(mem)
                 : "cc", "memory");
    return ret;
}

#endif