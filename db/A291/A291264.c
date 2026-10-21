/*
 * A291264  p-INVERT of (0,1,0,1,0,1,...), where p(S) = (1 - 2 S)^2.
 * https://oeis.org/A291264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=4, a(1)=12, a(2)=36, a(3)=104, a(4)=292 a(n) = 4*a(n-1) -
 *            2*a(n-2) - 4*a(n-3) - a(n-4) for n >= 5 This function memoises
 *            the sequence in a static array so that calls in any order are
 *            handled efficiently
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 95,
 *            big any n
 * Helpers:   alloc_seq, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"   /* alloc_seq(size_t n) -> seq_t* */
#include "../utils/seq_free.h"    /* seq_free(seq_t*) */

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Static table for memoisation */
    static seq_t *cache     = NULL;     /* array of computed terms          */
    static size_t cache_cap = 0;        /* allocated capacity               */
    static size_t cache_len = 0;        /* number of terms currently cached */

    /* Fast return if we already have the value */
    if (n < (idx_t)cache_len)
        return cache[(size_t)n];

    /* Initialise the first five terms on first use */
    if (cache_len == 0) {
        cache_cap = 5;
        cache     = alloc_seq(cache_cap);
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)104;
        cache[4] = (seq_t)292;
        cache_len = 5;
    }

    /* Grow the array if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;   /* double until big enough */

        seq_t *new_cache = alloc_seq(new_cap);
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        seq_free(cache);          /* free the old array */
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t term =
              (seq_t)4 * cache[i - 1]
            - (seq_t)2 * cache[i - 2]
            - (seq_t)4 * cache[i - 3]
            -          cache[i - 4];
        cache[i] = term;
    }

    /* Update known length to the largest computed index + 1 */
    if ((size_t)n + 1 > cache_len)
        cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
