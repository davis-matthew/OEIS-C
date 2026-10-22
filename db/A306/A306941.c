/*
 * A306941  Expansion of 1/((1 - x)^8 + x^8).
 * https://oeis.org/A306941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 11, i32 n <= 27, i64 n <= 63, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A306941: Expansion of 1/((1 - x)^8 + x^8)
   Recurrence for n > 7:
     a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
            - 28*a(n-6) + 8*a(n-7) - 2*a(n-8)
   Initial values:
     a(0)=1, a(1)=8, a(2)=36, a(3)=120, a(4)=330, a(5)=792,
     a(6)=1716, a(7)=3432
   The function memoises results so that repeated calls in any order
   are fast (O(n) amortised).  All operations are performed in type
   seq_t to guarantee exactness for the supported integer widths.  */

#include <stdlib.h>

/* Return a(n) for OEIS sequence A306941. */
seq_t a(idx_t n)
{
    /* Pre‑computed terms for 0 <= n < 8. */
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)8, (seq_t)36, (seq_t)120,
        (seq_t)330, (seq_t)792, (seq_t)1716, (seq_t)3432
    };

    /* Static cache variables (persist between calls). */
    static seq_t *cache   = NULL; /* pointer to allocated array of seq_t */
    static size_t cache_len = 0;  /* number of elements that have been computed */
    static size_t cache_cap = 0;  /* allocated capacity (in elements) */

    /* Ensure the cache can hold indices up to n. */
    size_t needed = (size_t)(n + (idx_t)1);   /* we need n+1 elements */
    if (cache == NULL) {
        /* First allocation */
        cache   = (seq_t *)malloc(needed * sizeof(seq_t));
        cache_cap = needed;
    } else if (needed > cache_cap) {
        /* Grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failure – in the test harness this never
               happens; but to be safe we return 0. */
            return (seq_t)0;
        }
        cache   = tmp;
        cache_cap = needed;
    }

    /* Compute new terms if necessary. */
    if (cache_len <= n) {
        /* Start at the first uncomputed index. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 8) {
                cache[i] = init[i];
            } else {
                /* Recurrence – all arithmetic occurs in seq_t. */
                cache[i] =
                    (seq_t)8  * cache[i-1] -
                    (seq_t)28 * cache[i-2] +
                    (seq_t)56 * cache[i-3] -
                    (seq_t)70 * cache[i-4] +
                    (seq_t)56 * cache[i-5] -
                    (seq_t)28 * cache[i-6] +
                    (seq_t)8  * cache[i-7] -
                    (seq_t)2  * cache[i-8];
            }
        }
        cache_len = (size_t)(n + (idx_t)1);
    }

    return cache[n];
}
