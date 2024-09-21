#include <linux/kernel.h>
#include <linux/memory.h>
#include <linux/vmalloc.h>
#include <asm/io.h>

#include "henry_virt_mem.h"

#include "henry_virt_debug.h"

int
henry_virt_mem_alloc(
    struct henry_virt_memory* mem,
    size_t mem_size
)
{
    if(unlikely(!mem))
        return -ENOMEM;

    mem->virt_mem = kmalloc(mem_size, GFP_KERNEL);
    if(unlikely(!mem))
        return -ENOMEM;
    
    mem->phy_mem = (void*)virt_to_phys(mem->virt_mem);
    return 0;
}

void
henry_virt_mem_free(
    struct henry_virt_memory* mem
)
{
    kfree(mem->virt_mem);
}

void
henry_virt_mem_aligned(
    struct henry_virt_memory in_mem,
    u64 align_size,
    struct henry_virt_memory* out_mem
)
{
    if(unlikely(!out_mem))
        return;

    // Example: align_size 4K (0x1000)
    // In virt mem: 0x12345678
    // In phy mem: 0x87654321
    // Out virt mem: (0x12345678 + 0xfff) & 0xfffff000 = 0x12346000
    // Max Size: 0x12345678 + 0x1000 = 0x12346678
    out_mem->phy_mem = (void*)((POINT_TO_INT(in_mem.phy_mem) + align_size - 1) & ~(align_size - 1));
    out_mem->virt_mem = (void*)((POINT_TO_INT(in_mem.virt_mem) + align_size - 1) & ~(align_size - 1));
}

void
henry_virt_mem_print(
    struct henry_virt_memory mem
)
{
    henry_virt_info("======Mem Info======\n");
    henry_virt_info("Virtual Memory Address: %x\n", (u32)POINT_TO_INT(mem.virt_mem));
    henry_virt_info("Physical Memory Address: %x\n", (u32)POINT_TO_INT(mem.phy_mem));
    henry_virt_info("======Mem Info======\n");
}