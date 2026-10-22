/*
 * A385980  a(0) = 1; a(n) = Sum_{k=0..n-1} (k+1) * binomial(k+3,3) *
 *          binomial(n-1,k) * a(k) * a(n-1-k).
 * https://oeis.org/A385980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} (k+1) * binomial(k+3,3) *
 *            binomial(n-1,k) * a(k) * a(n-1-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoization array */
static size_t cache_cap = 0;     /* allocated size of the cache */

/* ensure the cache is large enough for indices 0..n */
static void ensure_cache(idx_t n) {
    if ((idx_t)cache_cap > n) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* initialize newly allocated slots to 0 */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    ensure_cache(n);

    /* already computed? */
    if (cache[n] != 0) return cache[n];

    /* base case */
    if (n == 0) {
        cache[0] = 1;
        return cache[0];
    }

    /* compute all values up to n (dynamic programming) */
    for (idx_t m = 1; m <= n; ++m) {
        if (cache[m] != 0) continue;      /* already known */

        seq_t sum = 0;
        seq_t bin = 1;                     /* binomial(m-1,0) */

        for (idx_t k = 0; k < m; ++k) {
            /* compute (k+1)^2 * (k+3)(k+2) / 6 */
            seq_t term = ((seq_t)(k + 1)) * (seq_t)(k + 1) *
                         (seq_t)(k + 3) * (seq_t)(k + 2);
            term /= (seq_t)6;                /* exact division */

            seq_t prod = term * bin;
            prod *= cache[k];
            prod *= cache[m - 1 - k];
            sum += prod;

            /* update bin = binomial(m-1, k+1) for next k */
            if (k + 1 < m) {
                seq_t numerator = bin * (seq_t)(m - 1 - k);
                bin = numerator / (seq_t)(k + 1);
            }
        }
        cache[m] = sum;
    }

    return cache[n];
}
