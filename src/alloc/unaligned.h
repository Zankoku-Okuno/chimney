/// @file
/// @brief Interface for first-class allocators.
///
/// It is best practice in reusable C libraries to _not_ allocate on the heap internally.
/// When such libraries must have heap-allocated data, they should accept a set of allocator functions.
/// It can be a pain to pass such allocators around, and often libraries make different choices of interface.
///
/// This interface defines an allocator type with a single function pointer, making it easy to pass around.
/// New libraries (esp. my libraries) should use this type instead of re-inventing the wheel (again).
///
/// This is an interface for making unaligned memory allocations.
/// Some applications may require aligned memory allocation, which is provided in {@link ./aligned.h}.

#ifndef CHIM_ALLOC_UNALIGNED
#define CHIM_ALLOC_UNALIGNED

#ifndef INLINE
  #define INLINE inline
#endif

#include <stddef.h>


/// @brief First-class interface for memory management functions.
///
/// This interface is based on the C standard library `realloc`, which subsumes `malloc` and `free`.
/// Briefly: to allocate, pass `NULL` as the `ptr`, to deallocate, pass `0` as the `size`.
///
/// @param ptr: a pointer to a memory block (returned from a previous call), or `NULL`
/// @param size: size of the requested memory block, in bytes
/// @return a memory block of the requested size, or `NULL` it the request could not be satisfied
///
///   * If `ptr` is `NULL`, an unused block of memory will be allocated and returned.
///   * If `ptr` is non-null and `size` is zero, the memory block at `ptr` will be released and `NULL` returned.
///   * If `ptr is non-null and `size non-zero, the memory block will be re-allocated.
///       This could mean in-place resizing, or a move of the existing data to a new location.
///       A pointer to the new memory block is returned, and the passed pointer becomes invalidated
///         (which is _technically_ true even if the new memory block is the same as the old one).
///   * If at any time a request to (re)allocate cannot be satisfied, `NULL` is returned.
///
/// @see std_alloc: for the C Standard Library implementation of this interface
/// @see aligned_alloc_t: when the block has alignment requirements
typedef void* (*alloc_t)(void* ptr, size_t size);

/// @brief the C standard library `malloc` interface
extern const alloc_t std_alloc;

/// @brief allocate a new memory block
INLINE
void* allocIn(alloc_t allocator, size_t size) {
  return allocator(NULL, size);
}
/// @brief re-allocate a memory block
INLINE
void* reallocIn(alloc_t allocator, void* ptr, size_t size) {
  return allocator(ptr, size);
}
/// @brief release a memory block
INLINE
void freeIn(alloc_t allocator, void* ptr) {
  allocator(ptr, 0);
}


#endif
