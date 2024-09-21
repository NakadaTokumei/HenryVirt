#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/byteorder/generic.h>

#include "henry_virt_hyper.h"

#include "henry_virt_debug.h"

bool
check_intel_information()
{
    struct native_register reg = {0,};
    __henry_cpuid(0, &reg);

    henry_virt_info("CPU Information Value: [ebx: %u][ecx: %u][edx: %u]\n", reg.ebx, reg.ecx, reg.edx);

    return (reg.ebx == CPU_INFO_EBX) &&
           (reg.ecx == CPU_INFO_ECX) &&
           (reg.edx == CPU_INFO_EDX);
}

bool
check_vmx_supported()
{
    struct native_register reg = {0,};
    __henry_cpuid(1, &reg);

    henry_virt_info("CPU register value: [ecx: %u]\n", reg.ecx);

    return !!(reg.ecx & FEATURE_VMX_FLAG);
}

bool
enable_vmx()
{
    struct native_register reg = {0,};
    
    __henry_get_cr4(&reg);
    __henry_set_cr4(reg.cr4 | CR4_VMX_ENABLE_FLAG);
    __henry_get_cr4(&reg);

    henry_virt_info("CR4 VMX Enable Flag: [Value: %lld]\n", reg.cr4);

    return !!(reg.cr4 & CR4_VMX_ENABLE_FLAG);
}

static struct henry_virt_memory vmxon_mem;
struct henry_virt_memory vmxon_mem_aligned;

int
henry_virt_vmxon_region_alloc(void)
{
    int rc;
    rc = henry_virt_mem_alloc(&vmxon_mem, VMXON_SIZE + VMXON_ALIGN_SIZE);
    if(rc)
    {
        henry_virt_err("VMXON Region Allocation Failed...\n");
        return rc;
    }

    henry_virt_mem_aligned(
        vmxon_mem,
        VMXON_ALIGN_SIZE,
        &vmxon_mem_aligned
    );

    return 0;
}

int
henry_vmx_on(void)
{
    int rc;

    struct native_register reg;

    __henry_read_msr(IA32_VMX_BASIC, &reg);

    henry_virt_info("revision: %x\n", (u32)(reg.msr & 0x7fffffff));
    *(u64*)vmxon_mem_aligned.virt_mem = (u32)(reg.msr & 0x7fffffff);

    rc = __henry_vmxon(vmxon_mem_aligned.phy_mem);
    if(rc)
    {
        henry_virt_err("VMXON Failed... [rc: %u]\n", rc);
        return rc;
    }

    return 0;
}

static struct henry_virt_memory vmxcs_mem;
struct henry_virt_memory vmxcs_mem_aligned;

int
henry_virt_vmxcs_region_alloc(void)
{
    int rc;

    rc = henry_virt_mem_alloc(&vmxcs_mem, VMXCS_SIZE + VMXCS_ALIGN_SIZE);
    if(rc)
    {
        henry_virt_err("VMXCS Region Allocation Failed...\n");
        return rc;
    }

    henry_virt_mem_aligned(
        vmxcs_mem, 
        VMXCS_ALIGN_SIZE,
        &vmxcs_mem_aligned
    );

    return 0;
}

int
henry_vmx_cs_load(void)
{
    int rc;

    struct native_register reg;

    __henry_read_msr(IA32_VMX_BASIC, &reg);

    henry_virt_info("revision: %x\n", (u32)(reg.msr & 0x7fffffff));
    *(u64*)vmxcs_mem_aligned.virt_mem = (u32)(reg.msr & 0x7fffffff);

    rc = __henry_vmptrld(vmxcs_mem_aligned.phy_mem);
    if(rc)
    {
        henry_virt_err("VMXON Failed... [rc: %u]\n", rc);
        return rc;
    }

    return 0;
}

int
henry_virt_hyper_init(
    struct henry_virt_guest* virt_guest
)
{
    int rc;

    if (check_intel_information())
    {
        henry_virt_info("Intel information corrected");
    }
    else
    {
        rc = -EINVAL;
        return rc;
    }

    if (check_vmx_supported())
    {
        henry_virt_info("VMX Operation supported");
    }
    else
    {
        rc = -EINVAL;
        return rc;
    }

    if (enable_vmx())
    {
        henry_virt_info("VMX Enable success!");
    }
    else
    {
        rc = -EFAULT;
        return rc;
    }

    rc = henry_virt_vmxon_region_alloc();
    if (rc)
    {
        return rc;
    }

    rc = henry_vmx_on();
    if (rc)
    {
        return rc;
    }

    rc = henry_virt_vmxcs_region_alloc();
    if (rc)
    {
        return rc;
    }

    rc = henry_vmx_cs_load();
    if (rc)
    {
        return rc;
    }

    virt_guest->vmxon_region = &vmxon_mem_aligned;
    virt_guest->vmxcs_region = &vmxcs_mem_aligned;

    return 0;
}

void
henry_vmx_uninit()
{
    __henry_vmxoff();
    henry_virt_mem_free(&vmxon_mem);
    henry_virt_mem_free(&vmxcs_mem);
}