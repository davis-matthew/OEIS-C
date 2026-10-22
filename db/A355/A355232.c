/*
 * A355232  E.g.f. A(x) satisfies A'(x) = 1 + (exp(x) - 1) * A(2*x).
 * https://oeis.org/A355232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(n+1) = sum_{k=1..n-1} 2^k * binomial(n,k)
 *            * a(k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Dynamic cache of computed sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_last = 0;   /* largest index already computed */

/* Ensure the cache has room for indices up to n */
static void ensure_cache_size(idx_t n) {
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    ensure_cache_size(n);

    /* Initialise base values if needed */
    if (cache_last == 0) {
        cache[0] = 0;
        if (cache_cap <= 1) ensure_cache_size(1);
        cache[1] = 1;
        cache_last = 1;
    }

    /* Compute any missing values up to n */
    for (idx_t m = cache_last + 1; m <= n; ++m) {
        seq_t sum = 0;

        if (m >= 2) {
            seq_t binom   = 1;        /* C(m-1,0) */
            seq_t pow2k   = 2;        /* 2^1 */

            for (idx_t k = 1; k <= m - 2; ++k) {
                /* Update binomial: C(m-1, k) = C(m-1, k-1) * (m-k) / k */
                binom = binom * (seq_t)(m - k) / (seq_t)k;

                seq_t term = binom * cache[k];
                term *= pow2k;
                sum  += term;

                pow2k <<= 1;           /* multiply by 2 for next k */
            }
        }

        cache[m] = sum;
    }

    cache_last = n;
    return cache[n];
}
