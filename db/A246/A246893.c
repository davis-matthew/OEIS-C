/*
 * A246893  Number of length 1+4 0..n arrays with some pair in every
 *          consecutive five terms totalling exactly n.
 * https://oeis.org/A246893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 144, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A246893 – Number of length-1+4 0..n arrays with some pair in every consecutive
   five terms totalling exactly n.
   Recurrence (valid for n ≥ 9):
      a(n) = 2*a(n-1) + 2*a(n-2) – 6*a(n-3) + 6*a(n-5) – 2*a(n-6) – 2*a(n-7) + a(n-8)
   Initial values (a(1) … a(8)):
      30, 231, 900, 2701, 6210, 12931, 23400, 40281
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle non‑positive indices – behaviour below the offset is irrelevant */
    if (n < 1)
        return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of terms stored (last index+1) */
    static idx_t cache_cap  = 0; /* allocated capacity */

    /* Initial terms to seed the recurrence */
    static const seq_t init[8] = {
        (seq_t)30,  (seq_t)231, (seq_t)900,  (seq_t)2701,
        (seq_t)6210, (seq_t)12931, (seq_t)23400, (seq_t)40281
    };

    /* Allocate at least 8 slots and store the initial values */
    if (cache_cap < 8) {
        idx_t new_cap = 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* Allocation failed – fall back to zero */
            return (seq_t)0;
        }
        cache = tmp;
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_cap = new_cap;
        cache_size = 8;
    }

    /* Grow the cache if the requested index exceeds current capacity */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* Allocation failed – return zero */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= cache_size)
        return cache[n - 1];

    /* Compute missing terms using the recurrence */
    for (idx_t j = cache_size; j < n; ++j) {
        /* Indices: a(j+1) uses a(j) … a(j-7) */
        seq_t val = 2 * cache[j - 1]            /* 2*a(j)   */
                 + 2 * cache[j - 2]            /* 2*a(j-1) */
                 - 6 * cache[j - 3]            /* -6*a(j-2) */
                 + 6 * cache[j - 5]            /* +6*a(j-4) */
                 - 2 * cache[j - 6]            /* -2*a(j-5) */
                 - 2 * cache[j - 7]            /* -2*a(j-6) */
                 +     cache[j - 8];           /* +a(j-7) */
        cache[j] = val;
    }
    cache_size = n;

    return cache[n - 1];
}
