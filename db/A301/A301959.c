/*
 * A301959  Number of nX3 0..1 arrays with every element equal to 0, 1 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) + 3·a(n-2) + 2·a(n-3)
   a(1) = 3,   a(2) = 6,   a(3) = 16
   The function memoises results so that any order of calls
   is handled efficiently. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of terms already computed */
    static size_t  capacity   = 0;  /* allocated elements */

    if (n < 1) {
        /* sequence is not defined for n < 1 */
        return (seq_t)0;
    }

    /* initialise base terms on the first call */
    if (cached_len == 0) {
        const idx_t init_terms = 3;                /* a(1) .. a(3) */
        capacity = init_terms;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) {
            return (seq_t)0;                       /* out‑of‑memory guard */
        }
        cache[0] = (seq_t)3;   /* a(1) */
        cache[1] = (seq_t)6;   /* a(2) */
        cache[2] = (seq_t)16;  /* a(3) */
        cached_len = init_terms;
    }

    /* grow the cache if necessary to hold index n */
    if (n > capacity) {
        size_t new_cap = capacity;
        while (new_cap < n) {
            /* double until capacity ≥ n */
            new_cap = new_cap ? 2 * new_cap : 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            return (seq_t)0;   /* out‑of‑memory guard */
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* compute any terms that are still missing */
    for (idx_t i = cached_len + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) + 3·a(i-2) + 2·a(i-3) */
        seq_t term = cache[i - 2]                    /* a(i-1) */
                  + (seq_t)3 * cache[i - 3]          /* a(i-2) */
                  + (seq_t)2 * cache[i - 4];         /* a(i-3) */
        cache[i - 1] = term;                         /* store a(i) */
    }

    /* update the number of cached terms */
    if (n > cached_len) {
        cached_len = n;
    }

    return cache[n - 1];
}
