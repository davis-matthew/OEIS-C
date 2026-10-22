/*
 * A323263  The sequence denoted by d_n used in the calculation of A323260.
 * https://oeis.org/A323263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4*a(n-1) + 2*a(n-2) - 8*a(n-3) + 2*a(n-4) + 6*a(n-5) - a(n-6)
 * valid for n ≥ 11.  Base values (offset 1): 
 * a(1)=0, a(2)=0, a(3)=1, a(4)=3, a(5)=12, a(6)=49,
 * a(7)=197, a(8)=802, a(9)=3251, a(10)=13199
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values up to a(10) */
    static const seq_t base[11] = {
        0,     /* a(0) unused */
        0,     /* a(1) */
        0,     /* a(2) */
        1,     /* a(3) */
        3,     /* a(4) */
        12,    /* a(5) */
        49,    /* a(6) */
        197,   /* a(7) */
        802,   /* a(8) */
        3251,  /* a(9) */
        13199  /* a(10) */
    };

    if (n <= 10) {
        return base[(idx_t)n];
    }

    /* Static cache for memoization */
    static seq_t *cache      = NULL; /* array of computed values a[0..] */
    static size_t cache_cap  = 0;    /* allocated capacity          */
    static size_t cache_used = 0;    /* number of elements stored   */

    /* Ensure cache has room for index n */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)(n + 1)) {
            new_cap = (size_t)(n + 1);
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise cache with base values if not already done */
    if (cache_used < 11) {
        for (size_t i = cache_used; i < 11; ++i) {
            cache[i] = base[i];
        }
        cache_used = 11;
    }

    /* Fill cache up to n using recurrence */
    for (size_t i = cache_used; i <= (size_t)n; ++i) {
        /* Recurrence valid for i >= 11 */
        seq_t val = 0;
        val = val + (seq_t)4 * cache[i - 1];
        val = val + (seq_t)2 * cache[i - 2];
        val = val - (seq_t)8 * cache[i - 3];
        val = val + (seq_t)2 * cache[i - 4];
        val = val + (seq_t)6 * cache[i - 5];
        val = val - cache[i - 6];
        cache[i] = val;
    }
    cache_used = (size_t)n + 1;

    return cache[(size_t)n];
}
