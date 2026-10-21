/*
 * A246473  Number of length n+3 0..2 arrays with no pair in any consecutive
 *          four terms totalling exactly 2.
 * https://oeis.org/A246473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-4) with initial values: a(1)=10, a(2)=14,
 *            a(3)=20, a(4)=28
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 62, u64 n <= 131,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External function signature required by the framework */
seq_t a(idx_t n)
{
    /* All supported indices are >= 1.  For n==0, return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache holding already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;     /* highest n that has been computed */
    static size_t cache_cap = 0;  /* allocated array size (>= cache_n+1) */

    /* Ensure the cache has room for indices up to n (1‑based). */
    size_t needed = (size_t)n + 1;           /* index 0 unused */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Fill base cases if not yet done. */
    if (cache_n < 4) {
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)20;
        cache[4] = (seq_t)28;
        cache_n = 4;
    }

    /* Compute values from the first missing index up to n. */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 4];
    }
    cache_n = n;

    return cache[n];
}
