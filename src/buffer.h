/// @file
/// @brief Polymorphic resizable array list for C that keeps elements unboxed.
///
/// @todo _dynarr_index
///
/// ### Polymorphic Usage
///
/// Make sure that the corresponding C file is included in your build
///   (either by compiling as its own translation unit, or as part of a larger unit).
///
/// Then, instantiate this header at a type name with:
///
/// ```
/// #define DYNARR_TYPE <type name>
/// #include <this header>
/// ```
/// The type name must be an identifier, _not_ a type expression.
/// The name will be used to construct the names of functions.
///
/// It is not necessary to include the header without `DYNARR_TYPE` defined, nor should you include the C file with `DYNARR_TYPE` defined.
/// The header will automatically undefine `DYNARR_TYPE` when it is done.
///
/// After instantiation, identifiers of the form `/_dynarr(_<base name>)?/` in {@link buffer.h} are rewritten to
///   `dynarr(_<base name>)?_<type name>`.
/// However, some arguments (derivable from the type name) are removed from the argument list; these are marked _suppressed_.
/// For example, instantiating with a type name `int` will specialize {@link _dynarr_peek} to `int dynarr_peek_int(dynarr_int* arr)`.

#ifndef CHIM_BUFFER
#define CHIM_BUFFER

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "alloc/unaligned.h"


/// @brief Growable buffer.
///
/// This is a dynamically-resizable buffer of elements.
/// It grows automatically as elements are added.
typedef struct _dynarr {
  /// @brief capacity of the buffer
  size_t cap;
  /// @brief current length of buffer (not greater than the capacity)
  size_t len;
  /// @brief pointer to start of the buffered data
  char* buf;
} _dynarr;

/// @brief Initialize internal data structures.
///
/// If the initial capacity is zero, this likely indicatesa bug elsewhere, so initialization will fail.
///
/// @param mem: allocator
/// @param arr: the array
/// @param cap0: initial capacity (in elements)
/// @param size: (_suppressed_) size of element (in bytes)
/// @return false if allocation fails
// mallocs new internal data structures, and initialize length and capacity
// it does not attempt to clean up previous data
bool _dynarr_init(alloc_t mem, _dynarr* arr, size_t cap0, size_t size);

/// @brief Frees internal data structures used by the dynarr.
///
/// Makes no attempt to free any pointers owned by the elements.
///
/// @param mem: allocator
/// @param arr: the array
void _dynarr_deinit(alloc_t mem, _dynarr* arr);

/// @brief Copies an element to the end of the dynamic array.
///
/// The backing array is resized if necessary.
///
/// @param mem: allocator
/// @param arr: the array
/// @param elem: pointer to element
/// @param elemSize: (_suppressed_) size of an element, in bytes
/// @return false if allocation fails
bool _dynarr_push(alloc_t mem, _dynarr* arr, const void* elem, size_t elemSize);

/// @brief Return a reference to the last element of the array.
/// @param arr: the array
/// @param elemSize: (_suppressed_) size of an element, in bytes
/// @return reference to last element, or NULL if length is zero
void* _dynarr_peek(const _dynarr* arr, size_t elemSize);


/// @brief Remove the last element of the array and return a pointer to it.
///
/// @warning The caller must take ownership of the popped element, but
/// the pointer only stays valid until a new element is pushed, or the array is resized.
/// Thus, the caller should keep the returning poitner only temporarily.
///
/// @param arr: the array
/// @param elemSize: (_suppressed_) size of an element, in bytes
/// @return reference to last element, or NULL if length is zero
void* _dynarr_pop(_dynarr* arr, size_t elemSize);

/// @brief Grow or shrink the size of the buffer.
///
/// If the size is smaller than the current length, elements will be truncated off the array
/// As with {@link _dynarr_init}, the size cannot be zero.
///
/// @param mem: allocator
/// @param arr: the array
/// @param newCap: the requested new capacity of the array
/// @param elemSize: (_suppressed_) size of an element, in bytes
bool _dynarr_resize(alloc_t mem, _dynarr* arr, size_t newCap, size_t elemSize);

#endif




#ifdef DYNARR_TYPE
  // macros to paste expanded arguments
  #define _dynarr_paste(T) dynarr_ ## T
  #define _dynarr_init_paste(T) dynarr_init_ ## T
  #define _dynarr_deinit_paste(T) dynarr_deinit_ ## T
  #define _dynarr_push_paste(T) dynarr_push_ ## T
  #define _dynarr_peek_paste(T) dynarr_peek_ ## T
  #define _dynarr_pop_paste(T) dynarr_pop_ ## T
  #define _dynarr_resize_paste(T) dynarr_resize_ ## T
  // macros I actually use
  #define dynarr(T) _dynarr_paste(T)
  #define dynarr_init(T) _dynarr_init_paste(T)
  #define dynarr_deinit(T) _dynarr_deinit_paste(T)
  #define dynarr_push(T) _dynarr_push_paste(T)
  #define dynarr_peek(T) _dynarr_peek_paste(T)
  #define dynarr_pop(T) _dynarr_pop_paste(T)
  #define dynarr_resize(T) _dynarr_resize_paste(T)


typedef struct dynarr(DYNARR_TYPE) {
  size_t cap;
  size_t len;
  DYNARR_TYPE* buf;
} dynarr(DYNARR_TYPE);

// sanity check on compiler struct layout algorithm
static_assert(sizeof(dynarr(DYNARR_TYPE)) == sizeof(_dynarr)
             , "layout of polymorphic dynarr does not match _dynarr");
static_assert(offsetof(dynarr(DYNARR_TYPE), cap) == offsetof(_dynarr, cap)
             , "layout of polymorphic dynarr does not match _dynarr");
static_assert(offsetof(dynarr(DYNARR_TYPE), len) == offsetof(_dynarr, len)
             , "layout of polymorphic dynarr does not match _dynarr");
static_assert(offsetof(dynarr(DYNARR_TYPE), buf) == offsetof(_dynarr, buf)
             , "layout of polymorphic dynarr does not match _dynarr");


static inline
bool dynarr_init(DYNARR_TYPE)(alloc_t mem, dynarr(DYNARR_TYPE)* arr, size_t cap0) {
  return _dynarr_init(mem, (_dynarr*)arr, cap0, sizeof(DYNARR_TYPE));
}

static inline
void dynarr_deinit(DYNARR_TYPE)(alloc_t mem, dynarr(DYNARR_TYPE)* arr) {
  _dynarr_deinit(mem, (_dynarr*)arr);
}

static inline
bool dynarr_push(DYNARR_TYPE)(alloc_t mem, dynarr(DYNARR_TYPE)* arr, const DYNARR_TYPE* elem) {
  return _dynarr_push(mem, (_dynarr*)arr, (const void*)elem, sizeof(DYNARR_TYPE));
}

static inline
DYNARR_TYPE* dynarr_peek(DYNARR_TYPE)(const dynarr(DYNARR_TYPE)* arr) {
  return (DYNARR_TYPE*)_dynarr_peek((_dynarr*)arr, sizeof(DYNARR_TYPE));
}

static inline
DYNARR_TYPE* dynarr_pop(DYNARR_TYPE)(dynarr(DYNARR_TYPE)* arr) {
  return (DYNARR_TYPE*)_dynarr_pop((_dynarr*)arr, sizeof(DYNARR_TYPE));
}

static inline
bool dynarr_resize(DYNARR_TYPE)(alloc_t mem, dynarr(DYNARR_TYPE)* arr, size_t newCap) {
  return (DYNARR_TYPE*)_dynarr_resize(mem, (_dynarr*)arr, newCap, sizeof(DYNARR_TYPE));
}

  #undef dynarr
  #undef dynarr_init
  #undef dynarr_deinit
  #undef dynarr_push
  #undef dynarr_peek
  #undef dynarr_pop
  #undef _dynarr_paste
  #undef _dynarr_init_paste
  #undef _dynarr_deinit_paste
  #undef _dynarr_push_paste
  #undef _dynarr_peek_paste
  #undef _dynarr_pop_paste
  #undef DYNARR_TYPE
#endif
