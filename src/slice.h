/// @file
/// @brief Polymorphic slices of memory
///
/// ### Polymorphic Usage
///
/// Make sure that the corresponding C file is included in your build
///   (either by compiling as its own translation unit, or as part of a larger unit).
///
/// Then, instantiate this header at a type name with:
///
/// ```
/// #define LARR_TYPE <type name>
/// #include <this header>
/// ```
/// The type name must be an identifier, _not_ a type expression.
/// The name will be used to construct the names of functions.
///
/// It is not necessary to include the header without `LARR_TYPE` defined, nor should you include the C file with `LARR_TYPE` defined.
/// The header will automatically undefine `LARR_TYPE` when it is done.
///
/// After instantiation, identifiers of the form `/_lstr(_<base name>)?/` in {@link slice.h} are rewritten to
///   `lstr(_<base name>)?_<type name>`.
/// However, some arguments (derivable from the type name) are removed from the argument list; these are marked _suppressed_.
/// For example, instantiating with a type name `int` will specialize {@link _larr_addrof} to `int* lstr_addrof_int(lstr_int arr)`.

#ifndef CHIM_SLICE
#define CHIM_SLICE

#ifndef INLINE
  #define INLINE inline
#endif

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>


/// @brief array with length
typedef struct _larr {
  /// @brief length of the array
  size_t len;
  /// @brief pointer to the start of the array
  char* arr;
} _larr;

/// @brief Create an _larr
///
/// This exists primarily to work around C structure initialization being a statement instead of an expression.
///
/// @param len: the length of the array
/// @param arr: pointer to the start of the array
/// @return a length-array structure
INLINE
_larr _larr_mk(size_t len, void* arr) {
  _larr out = { .len = len, .arr = arr };
  return out;
}

/// @brief get the address corresponding to an index
///
/// This function performs bounds-checking.
/// If you're looking for unchecked array indexing… this is C, mate.
///
/// @param arr: the array to index
/// @param index: the index of the element
/// @param elemSize: (_suppressed_) the size of each element
/// @return the adderss of the `index`th element, or `NULL` if `index` is not in-bounds
INLINE
void* _larr_addrof(_larr arr, size_t index, size_t elemSize) {
  if (index < arr.len) {
    return arr.arr + elemSize * index;
  }
  else {
    return NULL;
  }
}

/// @brief Remove elements from the start of an array.
///
/// If the number of elements to remove is greater than the number of elements, the array is truncated to empty.
///
/// @param arr: the array to mutate
/// @param numElems: the number of elements to remove off the start
/// @param elemSize: the size of each element
INLINE
void _larr_advance(_larr* arr, size_t numElems, size_t elemSize) {
  if (numElems > arr->len) { numElems = arr->len; }
  arr->len -= numElems;
  arr->arr += elemSize * numElems;
}

/// @brief Remove elements from the end of an array.
///
/// If the number of elements to remove is greater than the number of elements, the array is truncated to empty.
/// This works by simply subtracting from the array's length.
///
/// @param arr: the array to mutate
/// @param numElems: the number of elements to remove off the end
INLINE
void _larr_shrink(_larr* arr, size_t numElems) {
  if (numElems > arr->len) { numElems = arr->len; }
  arr->len -= numElems;
}


#endif




#ifdef LARR_TYPE
  // macros to paste expanded arguments
  #define _larr_paste(T) larr_ ## T
  #define _larr_mk_paste(T) larr_mk_ ## T
  #define _larr_addrof_paste(T) larr_addrof_ ## T
  #define _larr_advance_paste(T) larr_advance_ ## T
  #define _larr_shrink_paste(T) larr_shrink_ ## T
  // macros I actually use
  #define larr(T) _larr_paste(T)
  #define larr_mk(T) _larr_mk_paste(T)
  #define larr_addrof(T) _larr_addrof_paste(T)
  #define larr_advance(T) _larr_advance_paste(T)
  #define larr_shrink(T) _larr_shrink_paste(T)

typedef struct larr(LARR_TYPE) {
  size_t len;
  LARR_TYPE* arr;
} larr(LARR_TYPE);

// sanity check on compiler struct layout algorithm
static_assert(sizeof(larr(LARR_TYPE)) == sizeof(_larr)
             , "layout of polymorphic larr does not match _larr");
static_assert(offsetof(larr(LARR_TYPE), len) == offsetof(_larr, len)
             , "layout of polymorphic larr does not match _larr");
static_assert(offsetof(larr(LARR_TYPE), arr) == offsetof(_larr, arr)
             , "layout of polymorphic larr does not match _larr");

static inline
larr(LARR_TYPE) larr_mk(LARR_TYPE)(size_t len, LARR_TYPE* arr) {
  larr(LARR_TYPE) out = { .len = len, .arr = arr };
  return out;
}

static inline
LARR_TYPE* larr_addrof(LARR_TYPE)(larr(LARR_TYPE) arr, size_t index) {
  _larr* arr_p = (_larr*)&arr;
  return _larr_addrof(*arr_p, index, sizeof(LARR_TYPE));
}

static inline
void larr_advance(LARR_TYPE)(larr(LARR_TYPE)* arr, size_t numElems) {
  _larr_advance((_larr*)arr, numElems, sizeof(LARR_TYPE));
}
static inline
void larr_shrink(LARR_TYPE)(larr(LARR_TYPE)* arr, size_t numElems) {
  _larr_shrink((_larr*)arr, numElems);
}

  #undef larr_shrink
  #undef larr_advance
  #undef larr_addrof
  #undef larr_mk
  #undef larr
  #undef _larr_shrink_paste
  #undef _larr_advance_paste
  #undef _larr_addrof_paste
  #undef _larr_mk_paste
  #undef _larr_paste
  #undef LARR_TYPE
#endif
