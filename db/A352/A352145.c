/*
 * A352145  Expansion of e.g.f. exp(-1 + cos(x) + sin(x)).
 * https://oeis.org/A352145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} (-1)^floor(k/2) *
 *            binomial(n-1,k-1) * a(n-k)
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 16, i64 n <= 26, i128 n <= 44,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_comp = 0;

seq_t a(idx_t n)
{
    if (!cache) {
        /* allocate space for at least the first element */
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = 1;
        last_comp = 0;
    }

    if ((idx_t)n > last_comp) {
        /* make sure we have room for indices up to n */
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t m = last_comp + 1; m <= n; ++m) {
            seq_t sum = 0;
            seq_t binom = 1;   /* binomial(m-1,0) */

            for (idx_t k = 1; k <= m; ++k) {
                seq_t term = binom * cache[m - k];

                /* sign = (-1)^floor(k/2) */
                if (((k >> 1) & 1) == 0)
                    sum += term;
                else
                    sum -= term;

                if (k < m) {
                    binom = (binom * (seq_t)(m - k)) / (seq_t)k;
                }
            }
            cache[m] = sum;
        }
        last_comp = n;
    }

    return cache[n];
}
