// memory.h

#ifndef _MEMORY_H
#define _MEMORY_H

#include "common.h"

i32 memory_total();

i32 memory_num_blocks();

void memory_print_info();

void* m_malloc(const u32 size);

void* m_calloc(const u32 size, const u32 count);

void* m_realloc(void* data, const u32 old_size, const u32 new_size);

void m_free(void* data, const u32 size);

#endif
