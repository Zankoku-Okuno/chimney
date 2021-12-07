/// @file
/// @brief Utilities for packing tag bits into aligned pointers.
///
/// If a pointer is sufficiently aligned, then we know that its low-order bits must be zero.
/// Thus, we can pack a little extra data into these bits, as long as we remember to strip them out before dereferencing.
///
/// This module defines a "native" tagged pointer type.
/// That is: the number of bits available to pack is just what is available in pointers returned by `malloc`.
/// If you require more bits, then you will have to use `aligned_alloc`,
/// which is more painful to work with, since there is no corresponding `aligned_realloc` in the standard library.

#ifndef CHIM_ALLOC_TAGS
#define CHIM_ALLOC_TAGS

#ifndef INLINE
  #define INLINE inline
#endif

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "chimtypes.h"

/// @brief Number of bits available to use as tag bits in pointers.
///
/// This relies on the size of `max_align_t`, which has the same alignment as for pointers allocated by `malloc`.
/// Thus, any `malloc`'d pointers will necessarily have this many low bits clear.
#define CHIM_PTRTAGBITS_MAX (__builtin_ctz(alignof(max_align_t)))

#define CHIM_PTRTAG_PTRMASK (~(uintptr_t)0 << CHIM_PTRTAGBITS_MAX)
#define CHIM_PTRTAG_BITSMASK (~CHIM_PTRTAG_PTRMASK)


/// @brief Tagged pointer type
///
/// Use this for tagged pointers to avoid accidentally dereferencing a tagged pointer without stripping the tag bits.
typedef bitsptr_t tagged_ptr;

/// @brief Tag a pointer.
/// @see setTag to alter tags on an existing tagged pointer
INLINE tagged_ptr to_tagged_ptr(void* ptr, uintptr_t tag);

/// @brief Strip tag from pointer.
INLINE void* unTag(tagged_ptr ptr);

/// @brief Extract tag from pointer.
INLINE uintptr_t getTag(tagged_ptr ptr);

/// @brief Alter the tag on an existing tagged pointer.
/// @see to_tagged_ptr to create a new tagged pointer
INLINE tagged_ptr setTag(tagged_ptr ptr, uintptr_t tag);

/// @brief Verify that the passed pointer is aligned appropriately for tagging.
INLINE bool is_taggable(void* ptr);


/// @brief Tag a pointer.
/// @see setTag to alter tags on an existing tagged pointer
INLINE tagged_ptr to_tagged_ptr(void* ptr, uintptr_t tag) {
  assert(is_taggable(ptr));
  bitsptr_t out = {.p = ptr};
  return setTag(out, tag);
}

/// @brief Strip tag from pointer.
INLINE void* unTag(tagged_ptr ptr) {
  bitsptr_t out = {.u = ptr.u & CHIM_PTRTAG_PTRMASK};
  return out.p;
}

/// @brief Extract tag from pointer.
INLINE uintptr_t getTag(tagged_ptr ptr) {
  return ptr.u & CHIM_PTRTAG_BITSMASK;
}

/// @brief Alter the tag on an existing tagged pointer.
/// @see to_tagged_ptr to create a new tagged pointer
INLINE tagged_ptr setTag(tagged_ptr ptr, uintptr_t tag) {
  assert((tag & CHIM_PTRTAGBITS_MAX) == 0);
  bitsptr_t out = {.u = ptr.u | tag};
  return out;
}

INLINE bool is_taggable(void* ptr) {
  bitsptr_t bits = {.p = ptr};
  return (bits.u & CHIM_PTRTAG_PTRMASK) == 0;
}


#endif
