/*
 * A291026  p-INVERT of the positive integers, where p(S) = 1 - 4*S + S^2.
 * https://oeis.org/A291026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 15*a(n-2) + 8*a(n-3) - a(n-4) Initial values:
 *            a(0) = 4 a(1) = 23 a(2) = 128 a(3) = 711
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values */
static idx_t cache_len = 0;   /* number of allocated slots in cache */
static idx_t cached_max = 0;  /* highest index that has been computed */

/* Ensure that cache can hold indices up to n */
static void ensure_capacity(idx_t n)
{
    if (cache_len <= n) {
        idx_t new_len = cache_len == 0 ? 4 : cache_len * 2;
        if (new_len <= n) new_len = n + 1;
        /* Use a cast so the code compiles in both C and C++ */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }
}

/* Compute the n-th term of A291026 */
seq_t a(idx_t n)
{
    /* Initialise base cases on first use */
    if (cache_len == 0) {
        ensure_capacity(3);
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)128;
        cache[3] = (seq_t)711;
        cached_max = 3;
    }

    /* If already computed, return the cached value */
    if (n <= cached_max)
        return cache[n];

    /* Extend cache and compute up to n */
    ensure_capacity(n);
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        cache[i] = (cache[i-1] * (seq_t)8)
                 - (cache[i-2] * (seq_t)15)
                 + (cache[i-3] * (seq_t)8)
                 - cache[i-4];
    }
    cached_max = n;
    return cache[n];
}
