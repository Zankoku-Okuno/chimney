# chimney (C sHIM library)

There are certain things in C that either come up all the time, and either have a poor API (e.g. error handling in i/o operations) or require a lot of untyped code (e.g. dynamic arrays).
This library is a collection of these things, as I run into them in my own work.


## Inventory

  * [x] static assert that you have a "normal" architecture
  * [x] bits types (i.e. convert between signed, unsigned, and (where applicable) pointer types)
  * [x] alignment arithmetic
  * [ ] `alloc/`: first-class allocation interfaces
    * [x] `unaligned`: unaligned allocations
    * [x] `aligned`: aligned allocations
    * [x] `tags`: tagged pointer utilites
      * [x] untyped native tagged pointers
      * [ ] polymorphic (`tagged_ptr<type ptr_type>`)
      * [ ] wider tags (set a tag width; would require aligned allocs)
      * [ ] polymorphic wider tags
    * [ ] polymorphic alloc
    * [ ] safe allocations: submit (programmer-controlled) size of object times (user-controlled) number of objects, detect overflows
  * [x] `buffer/`: polymorphic growable buffers
    * [x] forward (usual) buffer
    * [ ] backwards-growing buffer (for e.g. buffers of big-endian digits built little-endian)
    * [x] monomorphize to byte buffers
    * [x] monomorphize to `void*` buffers
    * [x] polymorphic pointer buffers
  * [x] memory slices
    * [x] length + pointer
      * [x] monomorphize to byte slices (lenstr)
      * [x] monomorphise to void* slices
      * [x] polymorphic pointer slices (lenarr)
    * [ ] original + offset + length
  * [ ] script that creates instantiations of polymorphic modules (so the documentation is better)
  * [ ] unicode utilities
    * [ ] a sentinel for char32_t
    * [ ] read utf8 from byte slice
    * [ ] create utf8 encoding for one character
  * other possibilities include (but I have not committed to)
    * sane i/o error reporting
    * list of blocks
    * decode binary-encoded integers from string/file (signed/unsigned 8,16,32,64-bit big/little-endian)
    * readline
  * runtime system utilites (these may go in here, or in an entirely separate library)
    * symbol table
    * garbage collector (simple and general object layout, generational, moving, single-threaded, some sort of inter-thread memory passing/sharing)
    * s-expressions
    * simple bigint library



## Macro Trickery


### Inline Functions

I want to write the body of an inline function exactly once, and emit exactly one copy.
To do this, I've followed [this advice](https://www.greenend.org.uk/rjk/tech/inline.html).

  * Header files are included as normal.
    To support this, the header has the preprocessor sequence: `#ifndef INLINE; #define INLINE inline; #endif`
    Then, the function must be _defined_ in that header file, preceded by`INLINE`.
    It may also declare the function, but this should be done either `inline` or `INLINE`.
  * Translation units must `#define INLINE` (i.e. define it as blank) before including their corresponding header.
    The translation unit does not itself define the inline function, since the ehader already defines it.

### Polymorphism

This is where the real trickery happens.
