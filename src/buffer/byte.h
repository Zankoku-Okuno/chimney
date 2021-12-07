/// @file
/// @brief {@link buffer.h} specialized to the byte type.
///
/// Byte buffers come up all the time, so it's nice to have this specialization already made.
///
/// This defines:
///   * dynarr_byte
///   * dynarr_init_byte
///   * dynarr_deinit_byte
///   * dynarr_push_byte
///   * dynarr_peek_byte
///   * dynarr_pop_byte
///   * dynarr_resize_byte

#ifndef CHIM_BUFFER_BYTE
#define CHIM_BUFFER_BYTE

#include "chimtypes.h"


#define DYNARR_TYPE byte
#include "buffer.h"


#endif
