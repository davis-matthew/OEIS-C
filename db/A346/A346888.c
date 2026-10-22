/*
 * A346888  Expansion of e.g.f. 1 / (1 - x^2 * exp(x) / 2).
 * https://oeis.org/A346888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A346888
   Sequence: 1, 0, 1, 3, 12, 70, 465, 3591, 31948, 319068, ...
   Formula (recurrence): a(0) = 1; for n >= 1,
        a(n) = Σ_{k=1..n} binomial(n,k) * binomial(k,2) * a(n-k).
   This implementation uses memoized dynamic programming and the
   utility function `binomial(idx_t n, idx_t k)` for exact integer
   binomial coefficients. */

#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cached_max = (idx_t)(-1);   /* -1 indicates uninitialized */

    /* Allocate or enlarge cache to hold values up to n */
    if (cache == NULL) {
        size_t new_cap = n + 1;
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!cache) abort();  /* allocation failure */
        /* initialize all entries to zero (good practice) */
        for (idx_t i = 0; i <= n; ++i) cache[i] = 0;
        cache[0] = 1;         /* a(0) = 1 */
        cached_max = 0;
    } else if (n > cached_max) {
        size_t new_cap = (cached_max + 1) * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();    /* reallocation failure */
        cache = tmp;
        /* initialize new entries to zero */
        for (idx_t i = cached_max + 1; i <= n; ++i) cache[i] = 0;
    }

    /* If values up to n are already computed, just return */
    if (n <= cached_max) return cache[n];

    /* Compute values from (cached_max+1) up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t bin_ik   = binomial(i, k);          /* binomial(i,k)   */
            seq_t bin_k2   = binomial(k, 2);          /* binomial(k,2)   */
            if (bin_k2 == 0) continue;               /* skip zero terms */
            seq_t prod = bin_ik;
            prod *= bin_k2;
            prod *= cache[i - k];
            sum += prod;
        }
        cache[i] = sum;
    }

    cached_max = n;
    return cache[n];
}
