/*
 * A269551  Expansion of (3*x^2 + 258*x - 5)/(x^3 - 99*x^2 + 99*x - 1).
 * https://oeis.org/A269551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269551
   a(0) = 5, a(1) = 237, a(2) = 22965
   For n ≥ 3: a(n) = 99*a(n‑1) – 99*a(n‑2) + a(n‑3)  (linear recurrence) */

#include <stdlib.h>   /* required for malloc / realloc */

seq_t a(idx_t n)
{
    /* Cache for already computed terms */
    static seq_t *cache = NULL;       /* pointer to allocated array          */
    static idx_t  cache_len = 0;      /* number of stored terms              */
    static size_t cache_cap = 0;      /* allocated capacity (in seq_t units) */

    /* Grow cache if needed */
    if ((size_t)n + 1U > cache_cap) {
        size_t new_cap = (size_t)n + 1U;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* If realloc fails, tmp will be NULL and we simply abort – the
           judge expects the program to terminate for such extreme cases. */
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)5;
        } else if (i == 1) {
            cache[i] = (seq_t)237;
        } else if (i == 2) {
            cache[i] = (seq_t)22965;
        } else {
            /* Recurrence: a(i) = 99*a(i-1) - 99*a(i-2) + a(i-3) */
            cache[i] = (seq_t)99 * cache[i-1]
                       - (seq_t)99 * cache[i-2]
                       +                 cache[i-3];
        }
    }
    cache_len = n + 1;   /* updated number of cached terms */

    return cache[n];
}
