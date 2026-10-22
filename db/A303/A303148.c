/*
 * A303148  Number of minimal total dominating sets in the n-pan graph.
 * https://oeis.org/A303148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 79, u64 n <= 157,
 *            u128 n <= 200, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A303148 – Number of minimal total dominating sets in the n-pan graph.
 * Recurrence (valid for n > 9):
 *   a(n) = a(n-3) + a(n-4) + a(n-5) + a(n-6) - a(n-8) - a(n-9)
 * Initial values:
 *   a(1)=1, a(2)=1, a(3)=3, a(4)=2, a(5)=4,
 *   a(6)=8, a(7)=6, a(8)=6, a(9)=13
 * The implementation keeps a static cache, allocating enough space as
 * needed.  It works for all supported seq_t types.
 */

#include <stddef.h>                 /* size_t */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*, size_t, size_t) */

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache     = NULL;          /* array of terms          */
    static size_t cache_sz = 0;              /* allocated capacity      */
    static idx_t  cache_max = 0;             /* highest index computed  */

    /* Ensure the array is large enough for the request */
    if ((size_t)n > cache_sz) {
        cache = seq_realloc(cache, cache_sz, (size_t)n);
        cache_sz = (size_t)n;
    }

    /* Initialize the first nine terms once */
    if (cache_max == 0) {
        if (cache_sz < 9) {                  /* need at least 9 slots   */
            cache = seq_realloc(cache, cache_sz, 9u);
            cache_sz = 9u;
        }
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)1;   /* a(2) */
        cache[2] = (seq_t)3;   /* a(3) */
        cache[3] = (seq_t)2;   /* a(4) */
        cache[4] = (seq_t)4;   /* a(5) */
        cache[5] = (seq_t)8;   /* a(6) */
        cache[6] = (seq_t)6;   /* a(7) */
        cache[7] = (seq_t)6;   /* a(8) */
        cache[8] = (seq_t)13;  /* a(9) */
        cache_max = 9;
    }

    /* If the term is already computed, return it */
    if (n <= cache_max) {
        return cache[n - 1];
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* All needed indices are already known because i > 9 */
        seq_t term =  cache[i - 3 - 1]    /* a(i-3) */
                    + cache[i - 4 - 1]    /* a(i-4) */
                    + cache[i - 5 - 1]    /* a(i-5) */
                    + cache[i - 6 - 1]    /* a(i-6) */
                    - cache[i - 8 - 1]    /* a(i-8) */
                    - cache[i - 9 - 1];   /* a(i-9) */
        cache[i - 1] = term;
    }

    cache_max = n;
    return cache[n - 1];
}
