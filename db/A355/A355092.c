/*
 * A355092  E.g.f. A(x) satisfies A(x) = 1 + 3 * (exp(x) - 1) * A(exp(x) - 1).
 * https://oeis.org/A355092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * Σ_{k=1..n} k * S2(n,k) * a(k-1), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache for computed a(n) values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;

    /* static array holding current Stirling‑2 row */
    static seq_t *S = NULL;
    static size_t S_cap = 0;

    /* initialise caches on first call */
    if (!cache) {
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = (seq_t)1;            /* a(0) = 1 */
    }
    if (!S) {
        ensure_cache(0, &S, &S_cap);
        S[0] = (seq_t)1;                /* S(0,0) = 1 */
    }

    if (n == 0)
        return cache[0];

    /* extend the sequence up to n if necessary */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* grow the Stirling array to hold index i */
            ensure_cache(i, &S, &S_cap);

            /* update Stirling numbers for row i
               iterate backwards to preserve old values         */
            for (idx_t k = i; k > 0; --k) {
                S[k] = (seq_t)k * S[k] + S[k - 1];
            }
            /* after the update, S[0] should be 0 for the next row */
            S[0] = (seq_t)0;

            /* compute a(i) from the recurrence */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = 3 * ((seq_t)k) * S[k] * cache[k - 1];
                sum += term;
            }
            cache[i] = sum;
        }
        max_computed = n;
    }

    return cache[n];
}
