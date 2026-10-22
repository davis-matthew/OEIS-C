/*
 * A363958  Expansion of (1 + x + x^3)/(1 - x^2 - 2*x^4 - 2*x^6 + x^8).
 * https://oeis.org/A363958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + 2*a(n-4) + 2*a(n-6) + a(n-8);
   initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=7, a(7)=10 */
#include <stdlib.h>

/* compute a(n) for OEIS sequence A363958 */
seq_t a(idx_t n)
{
    /* cache to store already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of slots currently allocated */
    static idx_t computed = 0;     /* number of entries that have been filled,
                                     i.e.   cache[0..computed-1] are valid   */

    /* if we already have the requested value, return it immediately */
    if (n < computed)
        return cache[n];

    /* enlarge the cache so that indices 0 .. n are valid */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();            /* out of memory: terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initial (known) values for n = 0..7 */
    static const seq_t init[8] = {1, 1, 1, 2, 3, 4, 7, 10};

    /* fill in any missing entries from previously uncomputed index up to n */
    for (idx_t k = computed; k <= n; ++k)
    {
        if (k < 8)
            cache[k] = init[k];
        else
            cache[k] = cache[k - 2]          /* a(k-2)  */
                       + (cache[k - 4] << 1)  /* 2 * a(k-4) */
                       + (cache[k - 6] << 1)  /* 2 * a(k-6) */
                       + cache[k - 8];        /* a(k-8)   */
    }

    /* mark that we have computed up to n inclusively */
    computed = n + 1;

    return cache[n];
}
