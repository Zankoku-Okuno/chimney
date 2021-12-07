/// @file
/// @brief Interface for first-class allocators, supporting alignement restrictions.
///
/// It is best practice in reusable C libraries to _not_ allocate on the heap internally.
/// When such libraries must have heap-allocated data, they should accept a set of allocator functions.
/// It can be a pain to pass such allocators around, and often libraries make different choices of interface.
///
/// This interface defines an allocator type with a single function pointer, making it easy to pass around.
/// New libraries (esp. my libraries) should use this type instead of re-inventing the wheel (again).
///
/// This is an interface for making aligned memory allocations.
/// Since the C Standard Library has limited support for this (relative to unaligned),
/// an unaligned interface, such as {@link ./unaligned.h} may be much more convenient.

#ifndef CHIM_ALLOC_ALIGNED
#define CHIM_ALLOC_ALIGNED

#ifndef INLINE
  #define INLINE inline
#endif

#include <stddef.h>


/// @brief First-class interface for aligned memory management functions
///
/// Exactly as {@link alloc_t}, but with support for specifying the desired alignment of the memory block.
///
/// @param ptr: pointer to existing memmory block, or `NULL` to allocate a new one
/// @param alignment: alignment if the requested memory block, in bytes
/// @param size: size of the requested memory block, in bytes
/// @return an pointer to an aligned memory block, or `NULL` if the request could not be satisfied
///
/// @see std_aalloc: for the C Standard Library implementation of this interface
/// @see alloc_t: when the block does not have alignment requirements
typedef void* (*aligned_alloc_t)(void* ptr, size_t alignment, size_t size);

/// @brief the C standard library `aligned_alloc` interface
///
/// @warning Since the C standard library has no `aligned_realloc` defined (at time of writing),
///   resizing a pointer through this interface is likely to be slower than the non-aligned interface {@link alloc_t}.
extern const aligned_alloc_t std_aalloc;

/// @brief allocate a new memory block
INLINE
void* aallocIn(aligned_alloc_t allocator, size_t alignment, size_t size) {
  return allocator(NULL, alignment, size);
}
/// @brief re-allocate a memory block
INLINE
void* areallocIn(aligned_alloc_t allocator, void* ptr, size_t alignment, size_t size) {
  return allocator(ptr, alignment, size);
}
/// @brief release a memory block
INLINE
void afreeIn(aligned_alloc_t allocator, void* ptr) {
  allocator(ptr, 1, 0);
}


#endif
