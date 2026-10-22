/*
 * A334282  Number of properly colored labeled graphs on n nodes so that the
 *          color function is surjective onto {c_1,c_2,...,c_k} for some k,
 *          1<=k<=n.
 * https://oeis.org/A334282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n-1} a(k) * binomial(n, k) * 2^{k(n-k)} with
 *            a(0) = 1. This recurrence follows from the generating function
 *            given in OEIS A334282
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b); */
#include "../utils/pow_uint.h"    /* seq_t pow_uint(uint64_t base, idx_t exp); */

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_size = 0;   /* next free index */

    if (!cache) {
        /* initialise cache for a(0) */
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* expand cache if needed */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* initialise new slots to zero */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            /* binomial(i, k) */
            seq_t coeff = binomial(i, k);
            /* 2^{k(i-k)} */
            idx_t exp = k * (i - k);
            seq_t pow2  = pow_uint(2, exp);
            /* term = a(k) * coeff * pow2 */
            seq_t term = cache[k];
            term = term * coeff;
            term = term * pow2;
            sum += term;
        }
        cache[i] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}
