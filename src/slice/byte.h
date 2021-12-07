/// @file
/// @brief {@link slice.h} specialized to the byte type.
///
/// Byte arrays (bytestrings) come up all the time, so it's nice to have this specialization already made.
///
/// This defines:
///   * larr_byte
///   * larr_addrof_byte

#ifndef CHIM_SLICE_BYTE
#define CHIM_SLICE_BYTE

#include "chimtypes.h"


#define LARR_TYPE byte
#include "slice.h"


#endif
