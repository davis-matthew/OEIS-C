/*
 * A288176  a(n) = 2*a(n-1) + 2*a(n-2) - 4*a(n-3) + a(n-4), where a(0) = 2,
 *          a(1) = 4, a(2) = 8, a(3) = 16.
 * https://oeis.org/A288176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 56, u128 n <= 114,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 2*a(n-2) - 4*a(n-3) + a(n-4)
 * with initial values a(0)=2, a(1)=4, a(2)=8, a(3)=16
 */
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* free() */
#include "../utils/alloc_seq.h"   /* alloc_seq() */

/* Computes the n-th term of OEIS sequence A288176 */
seq_t a(idx_t n)
{
    /* Persistent cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */

    /* Initialise the cache on first call */
    if (cache == NULL) {
        cache_len = 4;                                  /* a(0) .. a(3) */
        cache = alloc_seq(cache_len);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)16;
    }

    /* If the requested index is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend the cache to include the requested index */
    idx_t old_len = cache_len;
    cache_len = n + 1;
    seq_t *new_cache = alloc_seq(cache_len);

    /* Copy old values */
    for (idx_t i = 0; i < old_len; ++i) {
        new_cache[i] = cache[i];
    }

    /* Free the old array (associated with alloc_seq) */
    free(cache);

    /* Compute missing terms using the recurrence */
    for (idx_t i = old_len; i < cache_len; ++i) {
        new_cache[i] =
            (seq_t)2 * new_cache[i - 1] +
            (seq_t)2 * new_cache[i - 2] -
            (seq_t)4 * new_cache[i - 3] +
            (seq_t)1 * new_cache[i - 4];
    }

    cache = new_cache;
    return cache[n];
}
