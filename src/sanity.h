/// @file
/// @brief Check that the target machine is "sane"
///
/// By "sane", I really mean reasonably modern.
/// Essentially, programmers (like me) often make assumptions based on how computers are today.
/// Strictly by the C standard, these assumptions are unwarranted.
///
/// What kinds of assumptions are we talking about?
///  * a byte is eight bits
///  * two's complement
///  * memory space is flat (size_t/ptrdiff_t == uintptr_t/intptr_t, size_t and ptrdiff_t are the same width)
///
/// Note that some things are not checked:
///  * unsigned integer arithmetic wraps around (the standard specifies this)
///  * signed integer arithmetic wraps around (not testable: signed arithmetic can be treated differently in each context)
///  * precision is as expected (TODO: I'm not sure how to test this without a lot of busywork, see CERT C rule INT35-C)
///  * that convertions between pointers and uintptr_t is a no-op (i.e. cast-mask-cast is possible)

#include <assert.h>
#include <limits.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>


static_assert(CHAR_BIT == 8, "a byte is not eight bits on the target architecture");

/// @brief unsigned character
///
/// Because the C Standard doesn't specify whether a char is signed or unsigned, but it's very common to want an unsigned byte.
typedef unsigned char byte;


static_assert(-1 == ~0, "the target architecture is not two's-complement");


static_assert(sizeof(size_t) == sizeof(uintptr_t), "the target architecture is probably segmented (sizeof: size_t != uintptr_t)");
static_assert(alignof(size_t) == alignof(uintptr_t), "the target architecture is probably segmented (alignof: size_t != uintptr_t)");
static_assert(SIZE_MAX == UINTPTR_MAX, "the target architecture is probably segmented (max value: size_t != uintptr_t)");

static_assert(sizeof(ptrdiff_t) == sizeof(intptr_t), "the target architecture is probably segmented (sizeof ptrdiff_t != intptr_t)");
static_assert(alignof(ptrdiff_t) == alignof(intptr_t), "the target architecture is probably segmented (alignof ptrdiff_t != intptr_t)");
static_assert(PTRDIFF_MAX == INTPTR_MAX, "the target architecture is probably segmented (max value: ptrdiff_t != intptr_t)");
static_assert(PTRDIFF_MIN == INTPTR_MIN, "the target architecture is probably segmented (min value: ptrdiff_t != intptr_t)");

static_assert(sizeof(size_t) == sizeof(ptrdiff_t), "the target architecture is probably segmented (sizeof size_t != ptrdiff_t)");
static_assert(alignof(size_t) == alignof(ptrdiff_t), "the target architecture is probably segmented (alignof size_t != ptrdiff_t)");
