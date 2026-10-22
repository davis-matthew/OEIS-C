/*
 * A339027  E.g.f.: exp(2 * (exp(x) - 1 - x - x^2 / 2 - x^3 / 6 - x^4 / 24)).
 * https://oeis.org/A339027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=5..n} binomial(n‑1,k‑1) * a(n‑k), a(0) = 1,
 *            a(1)=a(2)=a(3)=a(4)=0
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 19, u64 n <= 29, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Static cache of computed values */
    static idx_t computed_max = 0;
    static size_t cache_cap   = 0;
    static seq_t *cache       = NULL;

    /* Allocate and initialise the cache on first use */
    if (!cache) {
        cache_cap = 8;                    /* at least enough for a(0)…a(7) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        memset(cache, 0, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;              /* a(0) */
        cache[1] = cache[2] = cache[3] = cache[4] = (seq_t)0;
        computed_max = 4;
    }

    /* Grow the cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* Newly allocated area is uninitialized but we will
           overwrite all entries we need below. */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i < 5) {
            cache[i] = (seq_t)0;          /* a(1) … a(4) */
        } else {
            seq_t sum = (seq_t)0;
            idx_t n_minus_1 = i - 1;
            /* Start with binomial(n-1,4) for k=5 */
            seq_t bin = binomial(n_minus_1, (idx_t)4);
            for (idx_t k = 5; k <= i; ++k) {
                /* term = binomial(n-1,k-1) * a(n-k) */
                seq_t term = bin * cache[i - k];
                sum += term;
                if (k < i) {
                    /* Update bin to binomial(n-1,(k+1)-1) */
                    idx_t numer = n_minus_1 - (k - 1); /* = n - k */
                    idx_t denom = k;
                    bin = (bin * (seq_t)numer) / (seq_t)denom;
                }
            }
            cache[i] = sum * (seq_t)2;
        }
    }
    computed_max = n;
    return cache[n];
}
