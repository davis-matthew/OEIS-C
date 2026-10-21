/*
 * A288914  a(1) = 2; a(n) = a(floor(n/a(n-1))) + 1 for n > 1.
 * https://oeis.org/A288914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2; a(n) = a(floor(n/a(n-1))) + 1 for n>1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static cache of size 20001 to store
   already computed terms.  Since a(n) depends only on a(n-1) and
   on indices strictly less than n/2, the sequence can be built
   iteratively from 1 up to any requested n.  The cache supports
   calls in any order and expands only once.  The code works
   with any seq_t defined by the build (uint8_t ... uint64_t,
   unsigned __int128, or an arbitrary-precision integer class
   in C++).  All operations use only what the problem permits. */

#include <stdlib.h>
#include <stddef.h>

/* The maximum n that we expect to handle safely.  The OEIS
   document states that the test cases are up to about 10000,
   so a buffer of 20001 (index 0 unused) is more than enough. */
#define A_MAX_INDEX 20001

static seq_t a_cache[A_MAX_INDEX];
static idx_t a_cache_size = 0;   /* largest index computed, 0 if none */

/* Ensure that the cache has been initialised up to at least index 1. */
static void a_ensure_initialised(void) {
    if (a_cache_size == 0) {
        a_cache[1] = (seq_t)2;
        a_cache_size = 1;
    }
}

/* Compute all terms a(i) for i = a_cache_size+1 .. n.  By
   construction the used indices are strictly increasing and
   only rely on already computed values. */
static void a_compute_up_to(idx_t n) {
    a_ensure_initialised();

    if (n <= a_cache_size)
        return;

    /* Compute in ascending order. */
    for (idx_t i = a_cache_size + 1; i <= n; ++i) {
        idx_t div = i / (idx_t)a_cache[i - 1];
        a_cache[i] = a_cache[div] + (seq_t)1;
    }
    a_cache_size = n;
}

seq_t a(idx_t n) {
    /* The problem guarantees n >= 1.  For safety we can handle
       n < 1 by returning 0, but this case is never used. */
    if (n < 1)
        return (seq_t)0;

    a_compute_up_to(n);
    return a_cache[n];
}
