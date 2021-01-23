// memory.c

#include "memory.h"

struct {
  i32 total;
  i32 blocks;
} memory_info;

#define memory_info_update(add_total, add_num_blocks) \
  memory_info.total += (add_total); \
  memory_info.blocks += (add_num_blocks)

i32 memory_total() {
  return memory_info.total;
}

i32 memory_num_blocks() {
  return memory_info.blocks;
}

void memory_print_info() {
  fprintf(stdout,
    "Memory info:\n  Allocated blocks: %i, Total: %.3g KB (%i bytes)\n",
    memory_info.blocks,
    memory_info.total / 1024.0f,
    memory_info.total
  );
}

void* m_malloc(const u32 size) {
  void* data = malloc(size);
  if (!data) return NULL;
  memory_info_update(size, 1);
  return data;
}

void* m_calloc(const u32 size, const u32 count) {
  void* data = calloc(size, count);
  if (!data) return NULL;
  memory_info_update(size * count, 1);
  return data;
}

void* m_realloc(void* data, const u32 old_size, const u32 new_size) {
  assert(data);
  i32 diff = new_size - old_size;
  void* temp = realloc(data, new_size);
  if (!temp)
    return NULL;
  memory_info_update(diff, 0);
  return temp;
}

void m_free(void* data, const u32 size) {
  assert(data);
  free(data);
  memory_info_update(-size, -1);
}
