/*
 * A339610  Expansion of x*(2 - x - x^2 - 2*x^3)/(1 - x - x^2)^2.
 * https://oeis.org/A339610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4), n>=5 Initial
 *            values: a(1)=2, a(2)=3, a(3)=7, a(4)=11
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization (1‑based index). */
static seq_t *cache = NULL;
static size_t  cache_cap = 0;
static idx_t   cache_len = 0;

/* Return the nth term of OEIS A339610. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n<1 */

    /* Initialise cache on the first call. */
    if (!cache) {
        cache_cap = 5;                        /* indices 0..4; ignore 0 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                  /* allocation failed */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)11;
        cache_len = 4;
    }

    /* Grow cache if necessary. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((uint64_t)new_cap <= n) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            /* Use a wider integer type only when seq_t is at most 8 bytes. */
            if (sizeof(seq_t) <= 8) {
                uint64_t t1 = (uint64_t)cache[i-1];
                uint64_t t2 = (uint64_t)cache[i-2];
                uint64_t t3 = (uint64_t)cache[i-3];
                uint64_t t4 = (uint64_t)cache[i-4];
                uint64_t tmp = t1 * 2 + t2 - t3 * 2 - t4;
                val = (seq_t)tmp;
            } else {
                /* For larger seq_t (arbitrary‑precision), use seq_t arithmetic. */
                val = cache[i-1] * 2 + cache[i-2] - cache[i-3] * 2 - cache[i-4];
            }
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
