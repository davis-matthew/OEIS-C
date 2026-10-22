/*
 * A355408  Expansion of e.g.f. 1/(1 + exp(x) - exp(3*x)).
 * https://oeis.org/A355408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=∑_{k=1..n} (3^k-1)·C(n,k)·a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_computed = 0; /* number of entries already stored */

    /* Make sure the array can hold indices 0..n. */
    ensure_cache((idx_t)(n + 1), &cache, &cache_cap);

    if (cache_computed <= (size_t)n) {
        for (idx_t i = cache_computed; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            seq_t binom = 1;          /* C(i,0) */
            seq_t pow3  = 3;          /* 3^1 for k = 1 */
            const seq_t one = 1;

            for (idx_t k = 1; k <= i; ++k) {
                /* Compute C(i,k) iteratively. */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

                seq_t term = binom * cache[i - k];
                seq_t factor = pow3 - one;  /* 3^k - 1 */
                term = term * factor;
                sum = sum + term;

                /* Advance to next power of three. */
                pow3 = pow3 * 3;
            }
            cache[i] = sum;
        }
        cache_computed = (size_t)n + 1;
    }

    return cache[n];
}
