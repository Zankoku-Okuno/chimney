/// @file
/// @brief Useful low-level types not provided by the standard
#ifndef CHIM_STDTYPES
#define CHIM_STDTYPES

#include <stdint.h>


/// @brief unsigned char
///
/// The standard does not specify the signedness of a character.
typedef unsigned char byte;

// I would define a `word` type, but which kind of word?
// Integer bus width? Address bus width? Floating point bus width? Some combination?
// Better to let such a type be defined by applications.

/// @brief void pointer
///
/// This is handy for polymorphism, such as in {@link buffer/boxed.h}.
typedef void* any;


/// @brief exactly eight bits of integral data, interpretable signed or unsigned
///
/// Undefined behavoir can occur when casting a signed integer type to unsigned
///   (e.g. on two's complement machines, when the integer is the minimum value).
/// My understanding is that reading through a union will not alter the underlying bits
///   (e.g. so that converting the above value through this union would result in the maximum unsigned value, as expected from modern machines).
typedef union bits8_t {
  /// @brief treat bits as an unsigned integer
  uint8_t u;
  /// @brief treat bits as a signed integer
  int8_t i;
} bits8_t;
/// @brief exactly sixteen bits of integral data, interpretable signed or unsigned
///
/// Undefined behavoir can occur when casting a signed integer type to unsigned
///   (e.g. on two's complement machines, when the integer is the minimum value).
/// My understanding is that reading through a union will not alter the underlying bits
///   (e.g. so that converting the above value through this union would result in the maximum unsigned value, as expected from modern machines).
typedef union bits16_t {
  /// @brief treat bits as an unsigned integer
  uint16_t u;
  /// @brief treat bits as a signed integer
  int16_t i;
} bits16_t;
/// @brief exactly thirty-two bits of integral data, interpretable signed or unsigned
///
/// Undefined behavoir can occur when casting a signed integer type to unsigned
///   (e.g. on two's complement machines, when the integer is the minimum value).
/// My understanding is that reading through a union will not alter the underlying bits
///   (e.g. so that converting the above value through this union would result in the maximum unsigned value, as expected from modern machines).
typedef union bits32_t {
  /// @brief treat bits as an unsigned integer
  uint32_t u;
  /// @brief treat bits as a signed integer
  int32_t i;
} bits32_t;
/// @brief exactly sixty-four bits of integral data, interpretable signed or unsigned
///
/// Undefined behavoir can occur when casting a signed integer type to unsigned
///   (e.g. on two's complement machines, when the integer is the minimum value).
/// My understanding is that reading through a union will not alter the underlying bits
///   (e.g. so that converting the above value through this union would result in the maximum unsigned value, as expected from modern machines).
typedef union bits64_t {
  /// @brief treat bits as an unsigned integer
  uint64_t u;
  /// @brief treat bits as a signed integer
  int64_t i;
} bits64_t;

///@brief enough bits to hold a pointer, also interpretable signed or unsigned.
///
/// Undefined behavoir can occur when casting a signed integer type to unsigned
///   (e.g. on two's complement machines, when the integer is the minimum value).
/// My understanding is that reading through a union will not alter the underlying bits
///   (e.g. so that converting the above value through this union would result in the maximum unsigned value, as expected from modern machines).
/// Similarly, the layout of the bits of a pointer is also not exactly defined, so performing non-pointer arithmetic on a pointer could benefit from this as well
///   (e.g. when tagging/untagging poitners).
typedef union bitsptr_t {
  /// @brief treat bits as an address
  void* p;
  /// @brief treat bits as an unsigned integer
  uintptr_t u;
  /// @brief treat bits as a signed integer
  intptr_t i;
} bitsptr_t;


#endif
