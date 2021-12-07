#include <stdlib.h>

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define INLINE
#include "aligned.h"


static inline
bool is_aligned(const void* restrict ptr, size_t log_alignment) {
  size_t mask = ((size_t)1 << log_alignment) - 1;
  return ((uintptr_t)ptr & mask) == 0;
}

static
void* aligned_realloc(void* ptr, size_t alignment, size_t size) {
  if (ptr == NULL) {
    return aligned_alloc(alignment, size);
  }
  else if (size == 0) {
    free(ptr);
    return NULL;
  }
  else {
    // TODO I'm not currently supporting alignment that isn't a power of two
    if (__builtin_popcount(alignment) != 1) {
      return NULL; // TODO I could work around this limitation
    }
    // In the worst-case, we may need to alloc a fresh aligned block and copy data into it.
    // I reserve that space now, because the realloc attempt may fail.
    // If I allocated this afterwards, the realloc would have altered the input,
    // but the interface for reallocation is to not touch the original if the request fails.
    void* new = aligned_alloc(alignment, size);
    if (new == NULL) { return NULL; }
    // First, we attempt to reallocate, since this might be in-place (very nice!)
    // or at least coincidentally return an aligned pointer (pretty good!).
    //
    // We also _need_ to reallocate so that we are assured of having enough bytes to copy out from.
    // That is, I'm not sure what the original size is, so a memcpy of the new size's length could read invalid memory.
    // By reallocating first, I am assured of having the data in a memory block of at least the new size's length.
    // 
    // TODO is this standards-compliant? (i.e. can I call realloc on an aligned_alloc pointer?
    void* attempt = realloc(ptr, size);
    if (is_aligned(attempt, __builtin_ctz(alignment))) {
      free(new);
      return attempt;
    } 
    else { // otherwise, we need to allocate an aligned block and move the data
      memcpy(new, attempt, size);
      free(attempt);
      return new;
    }
  }
}

const aligned_alloc_t std_aalloc = aligned_realloc;
