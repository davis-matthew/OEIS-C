/*
 * A333814  Multiples of 12 whose sum of digits is 12.
 * https://oeis.org/A333814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A333814  -  Multiples of 12 whose digit sum is 12
    Sequence definition (informal):
        a(n)  = the nth smallest positive integer of the form 12·k
                such that the sum of its decimal digits equals 12.
    The values are obtained by generating the multiples of 12 and
    selecting those that satisfy the digit–sum condition.  The function
    caches the terms that have been generated so far, so any order of
    calls with n ≥ 1 is handled correctly.  The cache grows only as
    needed. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"   /* provides: seq_t digit_sum(seq_t v) */

/* seq_t and idx_t types are supplied by the build system */

seq_t a(idx_t n)
{
    /* Static cache of previously found terms */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t cache_len = 0; /* number of terms currently cached */
    static size_t cache_cap = 0; /* capacity of the cache array */
    static idx_t next_k   = 1;    /* next multiplier of 12 to test */

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
    }

    /* Extend cache until at least n terms are known */
    while (cache_len < n) {
        /* candidate multiple of 12 */
        seq_t candidate = (seq_t)12 * (seq_t)next_k;

        /* check digit sum condition */
        if (digit_sum(candidate) == (seq_t)12) {
            /* store the candidate, resizing if necessary */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                cache = tmp;
            }
            cache[cache_len++] = candidate;
        }
        ++next_k;
    }

    return cache[n - 1];
}
