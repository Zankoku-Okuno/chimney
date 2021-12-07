/// @file
/// @brief Polymorphic array slice for C that boxes elements.
///
/// This piggybacks on the implementation in {@link slice.h} by specializing it to the `any` (`void*`) type.
/// Thus, it is also monomorphized to the `any` (`void*`) type, and defines:
///   * larr_any
///   * larr_mk_any
///   * larr_addrof_any
///   * larr_advance_any
///   * larr_shrink_any
///
/// ### Polymorphic Usage
///
/// Make sure that the corresponding `slice.c` is included in your build
///   (either by compiling as its own translation unit, or as part of a larger unit).
///
/// Then, instantiate this header at a type name with:
///
/// ```
/// #define LARRP_TYPE <type name>
/// #include <this header>
/// ```
/// The type name must be an identifier, _not_ a type expression.
/// The name will be used to construct the names of functions.
///
/// Including the header without `LARRP_TYPE` defined will only define the `larr_any` specialization of unboxed slices.
/// The header will automatically undefine `LARRP_TYPE` when it is done.
///
/// After instantiation, identifiers of the form `/_larr(_<base name>)?/` in {@link slice.h} are rewritten to
///   `larrp(_<base name>)?_<type name>`.
/// Arguments marked _suppressed_ are removed from the argument list, and the element type is passed/returned by pointer.
/// For example, instantiating with a type name `int` will specialize {@link _larr_addrof} to `int** lstr_addrof_int(lstrp_int arr)`.

#ifndef CHIM_SLICE_BOXED
#define CHIM_SLICE_BOXED

#include "chimtypes.h"

#define LARR_TYPE any
#include "slice.h"


#endif




#ifdef LARRP_TYPE
  // macros to paste expanded arguments
  #define _larrp_paste(T) larrp_ ## T
  #define _larrp_mk_paste(T) larrp_mk_ ## T
  #define _larrp_addrof_paste(T) larrp_addrof_ ## T
  #define _larrp_advance_paste(T) larrp_advance_ ## T
  #define _larrp_shrink_paste(T) larrp_shrink_ ## T
  // macros I actually use
  #define larrp(T) _larrp_paste(T)
  #define larrp_mk(T) _larrp_mk_paste(T)
  #define larrp_addrof(T) _larrp_addrof_paste(T)
  #define larrp_advance(T) _larrp_advance_paste(T)
  #define larrp_shrink(T) _larrp_shrink_paste(T)

typedef struct larrp(LARRP_TYPE) {
  size_t len;
  LARRP_TYPE** arr;
} larrp(LARRP_TYPE);

// sanity check on compiler struct layout algorithm
static_assert(sizeof(larrp(LARRP_TYPE)) == sizeof(_larr)
             , "layout of polymorphic larrp does not match _larr");
static_assert(offsetof(larrp(LARRP_TYPE), len) == offsetof(_larr, len)
             , "layout of polymorphic larrp does not match _larr");
static_assert(offsetof(larrp(LARRP_TYPE), arr) == offsetof(_larr, arr)
             , "layout of polymorphic larrp does not match _larr");

static inline
larrp(LARRP_TYPE) larrp_mk(LARRP_TYPE)(size_t len, LARRP_TYPE** arr) {
  larrp(LARRP_TYPE) out = { .len = len, .arr = arr };
  return out;
}

static inline
LARRP_TYPE** larrp_addrof(LARRP_TYPE)(larrp(LARRP_TYPE) arr, size_t index) {
  larr_any* arr_p = (larr_any*)&arr;
  return (LARRP_TYPE**)larr_addrof_any(*arr_p, index);
}

static inline
void larrp_advance(LARRP_TYPE)(larrp(LARRP_TYPE)* arr, size_t numElems) {
  larr_advance_any((larr_any*)arr, numElems);
}

static inline
void larrp_shrink(LARRP_TYPE)(larrp(LARRP_TYPE)* arr, size_t numElems) {
  larr_shrink_any((larr_any*)arr, numElems);
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
