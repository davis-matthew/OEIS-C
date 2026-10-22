/*
 * A331403  E.g.f.: 1 / ((1 + x) * sqrt(1 - 2*x)).
 * https://oeis.org/A331403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 0 Recurrence: a(n) = (n-1)*a(n-1) +
 *            (2*n-1)*(n-1)*a(n-2) for n ≥ 2 (derived from a(n) +
 *            (−n+1)*a(n-1) − (2*n-1)*(n-1)*a(n-2) = 0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS A331403 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of precomputed values */
    static size_t cache_cap = 0;    /* allocated capacity (in elements) */
    static size_t cache_size = 0;   /* number of values stored (next free index) */

    /* Initialise cache with the two base terms once */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* OOM guard */
        cache[0] = (seq_t)1;                  /* a(0) */
        cache[1] = (seq_t)0;                  /* a(1) */
        cache_size = 2;
    }

    /* Return immediately if the value is already computed */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Ensure the cache is large enough */
    size_t need = (size_t)n + 1;   /* indices 0 … n */
    if (need > cache_cap) {
        size_t newcap = cache_cap;
        while (newcap < need)
            newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* OOM guard */
        cache = tmp;
        cache_cap = newcap;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_size; i < need; ++i) {
        idx_t im1 = (idx_t)(i - 1);                     /* n-1 */
        idx_t two_i_minus1 = ((idx_t)2 * (idx_t)i) - (idx_t)1; /* 2*n-1 */

        seq_t term1 = ((seq_t)im1) * cache[i - 1];
        seq_t term2 = ((seq_t)im1) * ((seq_t)two_i_minus1) * cache[i - 2];
        cache[i] = term1 + term2;
    }

    cache_size = need;
    return cache[(size_t)n];
}
