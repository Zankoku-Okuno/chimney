/// @file
/// @brief Alignment arithmetic

#ifndef CHIM_ALIGNMENT
#define CHIM_ALIGNMENT

#ifndef INLINE
  #define INLINE inline
#endif

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


/// @brief Increment a number so that it is divisible by a power of two
///
/// Particularly useful for pointer arithmetic:
///   e.g. if an allocator needs to increase the requested size so that it fits a known block size.
///
/// @param bits: the number to be aligned
/// @param alignment_pow2: a power of two to align to
///   @warning if `alignment_pow2` is not a power of two, the result is undefined
/// @return the smallest number at least as large as the input which is divisible by the power of two
uintptr_t alignUp(uintptr_t bits, size_t alignment_pow2) {
  assert(__builtin_popcount(alignment_pow2) == 1);
  uintptr_t mask = alignment_pow2 - 1;
  if ((bits & mask) != 0) {
    bits += (~bits & mask) + 1;
  }
  return bits;
}

/// @brief Decrement a number so that it is divisible by a power of two
///
/// Particularly useful for pointer arithmetic:
///   e.g. if an object is known to be in an aligned block which has a header.
///
/// @param bits: the number to be aligned
/// @param alignment_pow2: a power of two to align to
///   @warning if `alignment_pow2` is not a power of two, the result is undefined
/// @return the largest number no larger than the input which is divisible by the power of two
uintptr_t alignDown(uintptr_t bits, size_t alignment_pow2) {
  assert(__builtin_popcount(alignment_pow2) == 1);
  uintptr_t mask = alignment_pow2 - 1;
  return bits & ~mask;
}


#endif
