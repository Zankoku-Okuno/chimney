/// @file
/// @brief Polymorphic resizable array list for C that boxes elements.
///
/// This piggybacks on the implementation in {@link buffer.h} by specializing it to the `any` (`void*`) type.
/// Thus, it is also monomorphized to the `any` (`void*`) type, and defines:
///   * dynarr_any
///   * dynarr_init_any
///   * dynarr_deinit_any
///   * dynarr_push_any
///   * dynarr_peek_any
///   * dynarr_pop_any
///   * dynarr_resize_any
///
/// ### Polymorphic Usage
///
/// Make sure that the corresponding `buffer.c` is included in your build
///   (either by compiling as its own translation unit, or as part of a larger unit).
///
/// Then, instantiate this header at a type name with:
///
/// ```
/// #define DYNARRP_TYPE <type name>
/// #include <this header>
/// ```
/// The type name must be an identifier, _not_ a type expression.
/// The name will be used to construct the names of functions.
///
/// Including the header without `DYNARRP_TYPE` defined will only define the `dynarr_any` specialization of unboxed buffers.
/// The header will automatically undefine `DYNARRP_TYPE` when it is done.
///
/// After instantiation, identifiers of the form `/_dynarr(_<base name>)?/` in {@link buffer.h} are rewritten to
///   `dynarrp(_<base name>)?_<type name>`.
/// Arguments marked _suppressed_ are removed from the argument list, and the element type is passed/returned by pointer.
/// For example, instantiating with a type name `int` will specialize {@link _dynarr_peek} as `int* dynarrp_peek_int(dynarrp_int* arr)`.

#ifndef CHIM_BUFFER_BOXED
#define CHIM_BUFFER_BOXED


#include "chimtypes.h"

#define DYNARR_TYPE any
#include "buffer.h"


#endif


#ifdef DYNARRP_TYPE
  // macros to paste expanded arguments
  #define _dynarrp_paste(T) dynarrp_ ## T
  #define _dynarrp_init_paste(T) dynarrp_init_ ## T
  #define _dynarrp_deinit_paste(T) dynarrp_deinit_ ## T
  #define _dynarrp_push_paste(T) dynarrp_push_ ## T
  #define _dynarrp_peek_paste(T) dynarrp_peek_ ## T
  #define _dynarrp_pop_paste(T) dynarrp_pop_ ## T
  #define _dynarrp_resize_paste(T) dynarrp_resize_ ## T
  // macros I actually use
  #define dynarrp(T) _dynarrp_paste(T)
  #define dynarrp_init(T) _dynarrp_init_paste(T)
  #define dynarrp_deinit(T) _dynarrp_deinit_paste(T)
  #define dynarrp_push(T) _dynarrp_push_paste(T)
  #define dynarrp_peek(T) _dynarrp_peek_paste(T)
  #define dynarrp_pop(T) _dynarrp_pop_paste(T)
  #define dynarrp_resize(T) _dynarrp_resize_paste(T)


typedef struct dynarrp(DYNARRP_TYPE) {
  size_t cap;
  size_t len;
  DYNARRP_TYPE** data;
} dynarrp(DYNARRP_TYPE);

// sanity check on compiler struct layout algorithm
static_assert(offsetof(dynarrp(DYNARRP_TYPE), cap) == offsetof(_dynarr, cap)
             , "layout of polymorphic dynarr does not match _dynarr");
static_assert(offsetof(dynarrp(DYNARRP_TYPE), len) == offsetof(_dynarr, len)
             , "layout of polymorphic dynarr does not match _dynarr");
static_assert(offsetof(dynarrp(DYNARRP_TYPE), data) == offsetof(_dynarr, data)
             , "layout of polymorphic dynarr does not match _dynarr");

static inline
bool dynarrp_init(DYNARRP_TYPE)(alloc_t mem, dynarrp(DYNARRP_TYPE)* arr, size_t cap0) {
  return _dynarr_init(mem, (_dynarr*)arr, cap0, sizeof(DYNARRP_TYPE*));
}

static inline
void dynarrp_deinit(DYNARRP_TYPE)(alloc_t mem, dynarrp(DYNARRP_TYPE)* arr) {
  _dynarr_deinit(mem, (_dynarr*)arr);
}

static inline
bool dynarrp_push(DYNARRP_TYPE)(alloc_t mem, dynarrp(DYNARRP_TYPE)* arr, const DYNARRP_TYPE* elem) {
  return _dynarr_push(mem, (_dynarr*)arr, (const void*)elem, sizeof(DYNARRP_TYPE*));
}

static inline
DYNARRP_TYPE* dynarrp_peek(DYNARRP_TYPE)(const dynarrp(DYNARRP_TYPE)* arr) {
  return (DYNARRP_TYPE*)_dynarr_peek((_dynarr*)arr, sizeof(DYNARRP_TYPE*));
}

static inline
DYNARRP_TYPE* dynarrp_pop(DYNARRP_TYPE)(dynarrp(DYNARRP_TYPE)* arr) {
  return (DYNARRP_TYPE*)_dynarr_pop((_dynarr*)arr, sizeof(DYNARRP_TYPE*));
}

static inline
bool dynarrp_resize(DYNARRP_TYPE)(alloc_t mem, dynarrp(DYNARRP_TYPE)* arr, size_t newCap) {
  return (DYNARRP_TYPE*)_dynarr_resize(mem, (_dynarr*)arr, newCap, sizeof(DYNARRP_TYPE*));
}

  #undef dynarrp
  #undef dynarrp_init
  #undef dynarrp_deinit
  #undef dynarrp_push
  #undef dynarrp_peek
  #undef dynarrp_pop
  #undef _dynarrp_paste
  #undef _dynarrp_init_paste
  #undef _dynarrp_deinit_paste
  #undef _dynarrp_push_paste
  #undef _dynarrp_peek_paste
  #undef _dynarrp_pop_paste
  #undef DYNARRP_TYPE
#endif
