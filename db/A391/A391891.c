/*
 * A391891  a(n) = Sum_{k=0..n} (k+1) * binomial(2*k+1,2*n-2*k).
 * https://oeis.org/A391891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * binomial(2*k+1, 2*n-2*k) Recurrence
 *            (for n ≥ 8): a(n) = 4*a(n-1) - 2*a(n-2) - 11*a(n-4) - 2*a(n-6) +
 *            4*a(n-7) - a(n-8) Initial values: a(0)=1, a(1)=2, a(2)=9,
 *            a(3)=34, a(4)=104, a(5)=326, a(6)=995, a(7)=2954
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: dynamic array of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* hard‑coded first eight terms */
    static const seq_t init_vals[8] = {
        (seq_t)1,  (seq_t)2,  (seq_t)9,  (seq_t)34,
        (seq_t)104, (seq_t)326, (seq_t)995, (seq_t)2954
    };

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 16;                      /* arbitrary starting size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure → 0 */
        for (size_t i = 0; i < 8; ++i) cache[i] = init_vals[i];
        cache_len = 8;
    }

    /* if the requested index is below the seed, return directly */
    if (n < 8) {
        return init_vals[(size_t)n];
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;  /* double until enough */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure → 0 */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing entries up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t term = 4 * cache[i-1] + 4 * cache[i-7];
        seq_t sub  = 2 * cache[i-2] + 11 * cache[i-4]
                   + 2 * cache[i-6] +      cache[i-8];
        cache[i] = term - sub; /* unsigned arithmetic; guaranteed non‑negative */
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
