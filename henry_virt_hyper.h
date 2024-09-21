#ifndef __HENRY_VIRT_HYPER_H__
#define __HENRY_VIRT_HYPER_H__

#include "henry_virt_asm.h"
#include "henry_virt_mem.h"
#include "henry_virt_register.h"

#define CPU_INFO_EBX ((u32)1970169159)
#define CPU_INFO_ECX ((u32)1818588270)
#define CPU_INFO_EDX ((u32)1231384169)


#define FEATURE_VMX_FLAG    ((u32)(0x01 << 5))

#define CR4_VMX_ENABLE_FLAG ((u64)(0x01 << 13))

#define VMXON_ALIGN_SIZE    ((u64)0x1000)

#define VMXCS_ALIGN_SIZE    VMXON_ALIGN_SIZE

struct henry_virt_vmcs {
    u64 reserved;
};
#define VMXCS_SIZE (sizeof(struct henry_virt_vmcs))

struct henry_virt_vmxon {
    u64 reserved;
};
#define VMXON_SIZE (sizeof(struct henry_virt_vmxon))

struct henry_virt_guest {
    struct henry_virt_memory* vmxon_region;
    struct henry_virt_memory* vmxcs_region;
};

bool
check_intel_information(void);

bool
check_vmx_supported(void);

bool
enable_vmx(void);

int
henry_virt_vmxon_region_alloc(void);

int
henry_virt_vmxcs_region_alloc(void);

int
henry_vmx_on(void);

int
henry_vmx_cs_load(void);

int
henry_virt_hyper_init(
    struct henry_virt_guest* virt_guest
);

void
henry_vmx_uninit(void);

#endif