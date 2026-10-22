/*
 * A305036  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 5*a(n-3) + a(n-4) - 4*a(n-6) - 4*a(n-7) for n >
 *            11 a(1) = 8, a(2) = 5, a(3) = 7, a(4) = 19, a(5) = 26, a(6) =
 *            61, a(7) = 143, a(8) = 277, a(9) = 568, a(10) = 1244, a(11) =
 *            2600
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic cache of computed values */
static size_t cache_cap = 0;     /* current capacity of cache   */
static idx_t  cache_last = 0;    /* largest n already computed  */

/* Helper to ensure the cache can hold values up to n (inclusive) */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* The sequence function */
seq_t a(idx_t n)
{
    /* table of the first 11 terms (index 1..11) */
    static const seq_t base[12] = {
        0,          /* unused 0th element */
        8, 5, 7, 19, 26, 61, 143, 277, 568, 1244, 2600
    };

    if (n == 0) return 0;   /* n>=1 by definition, but safe guard */

    /* Initialise cache on first call */
    if (cache == NULL) {
        ensure_cache_capacity(11);
        for (idx_t i = 1; i <= 11; ++i)
            cache[i] = base[i];
        cache_last = 11;
    }

    /* Return known values directly */
    if (n <= cache_last)
        return cache[n];

    /* Compute values up to n using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        ensure_cache_capacity(i);      /* extend cache if needed */
        seq_t sum1 = cache[i-1]            /* a(i-1)   */
                   + 5 * cache[i-3]        /* 5*a(i-3) */
                   + cache[i-4];           /* + a(i-4) */
        seq_t sum2 = 4 * cache[i-6]        /* 4*a(i-6) */
                   + 4 * cache[i-7];       /* + 4*a(i-7) */
        cache[i] = sum1 - sum2;            /* a(i) = sum1 - sum2 */
    }
    cache_last = n;

    return cache[n];
}
