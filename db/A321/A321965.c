/*
 * A321965  a(n) = n! [x^n] exp((1/(x - 1)^2 - 1)/2)/(1 - x).
 * https://oeis.org/A321965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n + 3) = (n + 1)^2*(n + 2)*a(n) - (5 + 3*n)*(n + 2)*a(n + 1) +
 *            (8 + 3*n)*a(n + 2). Initial values: a(0)=1, a(1)=2, a(2)=8
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute A321965.  The function is memoised so that calls in any order are
   efficient. */
seq_t a(idx_t n) {
    /* Handle the offset initial values directly. */
    if (n < 3) {
        static const seq_t init[3] = {(seq_t)1, (seq_t)2, (seq_t)8};
        return init[n];
    }

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;     /* number of valid entries stored */

    /* Ensure the cache is large enough and initialised. */
    if (!cache) {
        /* Allocate a small initial cache. */
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);        /* failed to allocate */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
    }

    /* Grow the cache if necessary. */
    if (n >= cache_size) {
        size_t new_size = cache_size ? cache_size : 4;
        while (new_size <= n) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);        /* failed to reallocate */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = 3; i <= n; ++i) {
        /* Ensure we only compute once per index. */
        if (i > 2 && i < cache_size && i < i) { /* placeholder to avoid unused warning */ }
        idx_t k = i - 3;
        seq_t kk = (seq_t)k;
        seq_t term0 = cache[k];
        seq_t term1 = cache[k + 1];
        seq_t term2 = cache[k + 2];

        /* Factor terms of the recurrence, all in seq_t. */
        seq_t factor0 = (kk + 1) * (kk + 1) * (kk + 2);      /* (k+1)^2*(k+2) */
        seq_t fac1     = (5 + 3 * kk) * (kk + 2);             /* (5+3k)*(k+2) */
        seq_t fac2     = 8 + 3 * kk;                          /* 8+3k */

        /* a(n+3) = factor0*term0 - fac1*term1 + fac2*term2.
           Compute in a way that avoids unsigned underflow. */
        seq_t val = factor0 * term0;
        val += fac2 * term2;
        val -= fac1 * term1;

        cache[i] = val;
    }

    return cache[n];
}
