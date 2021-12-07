#include "buffer.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool _dynarr_init(alloc_t mem, _dynarr* arr, size_t cap0, size_t size) {
  if (cap0 == 0) { return false; }
  if (cap0 * size < size) { return false; }
  arr->buf = allocIn(mem, cap0 * size);
  if (arr->buf == NULL) { return false; }
  arr->cap = cap0;
  arr->len = 0;
  return true;
}

void _dynarr_deinit(alloc_t mem, _dynarr* arr) {
  arr->cap = 0;
  arr->len = 0;
  freeIn(mem, arr->buf);
  arr->buf = NULL;
}

bool _dynarr_push(alloc_t mem, _dynarr* arr, const void* elem, size_t elemSize) {
  assert(arr->cap != 0);
  if (arr->len == arr->cap) {
    if (arr->cap >= SIZE_MAX/2) { return false; }
    arr->cap *= 2;
    char* new = reallocIn(mem, arr->buf, arr->cap * elemSize);
    if (arr->buf == NULL) { return false; }
    arr->buf = new;
  }
  memcpy(&arr->buf[elemSize * arr->len], elem, elemSize);
  arr->len += 1;
  return true;
}

void* _dynarr_peek(const _dynarr* arr, size_t elemSize) {
  if (arr->len == 0) { return NULL; }
  return &arr->buf[elemSize * (arr->len - 1)];
}

void* _dynarr_pop(_dynarr* arr, size_t elemSize) {
  if (arr->len == 0) { return NULL; }
  arr->len -= 1;
  return &arr->buf[elemSize * arr->len];
}

bool _dynarr_resize(alloc_t mem, _dynarr* arr, size_t newCap, size_t elemSize) {
  if (newCap == 0) { return false; }
  char* new = reallocIn(mem, arr->buf, arr->cap * elemSize);
  if (new == NULL) { return false; }
  arr->cap = newCap;
  if (newCap < arr->len) {
    arr->len = newCap;
  }
  arr->buf = new;
  return true;
}
