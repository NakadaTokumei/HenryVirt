#ifndef __HENRY_VIRT_MEM_H__
#define __HENRY_VIRT_MEM_H__

#include <linux/types.h>

#define POINT_TO_INT(X) ((u64)X)

struct henry_virt_memory {
    void* virt_mem;
    void* phy_mem;
};

int
henry_virt_mem_alloc(
    struct henry_virt_memory* mem,
    size_t mem_size
);

void
henry_virt_mem_free(
    struct henry_virt_memory* mem
);

void
henry_virt_mem_print(
    struct henry_virt_memory mem
);

void
henry_virt_mem_aligned(
    struct henry_virt_memory in_mem,
    u64 align_size,
    struct henry_virt_memory* out_mem
);

#endif