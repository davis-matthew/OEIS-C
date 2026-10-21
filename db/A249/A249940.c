/*
 * A249940  E.g.f.: 1 + Sum_{n>=1} 2*exp(n^2*x) / 2^n.
 * https://oeis.org/A249940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Σ_{k=1}^n binom(2n, 2k) * a(n-k), a(0) = 3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_new_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of computed entries */

    /* First call : initialise a(0) and prepare storage for the requested n */
    if (cache_size == 0) {
        cache = seq_alloc_new_array(n + 1);
        cache[0] = (seq_t)3;       /* a(0) = 3 */
        cache_size = 1;
    }

    /* Compute new values up to n if necessary */
    if (n >= cache_size) {
        /* Extend the cache to hold entries up to n */
        idx_t oldsize  = cache_size;
        idx_t newsize  = n + 1;
        seq_t *newcache = seq_alloc_new_array(newsize);

        /* Copy previously computed values */
        for (idx_t i = 0; i < oldsize; ++i)
            newcache[i] = cache[i];

        /* Compute a(m) for m = oldsize .. n */
        for (idx_t m = oldsize; m <= n; ++m) {
            seq_t sum = 0;        /* Σ_{k=1..m} binom(2m, 2k) * a(m-k) */
            seq_t C   = 1;        /* binom(2m, 0) */

            for (idx_t k = 1; k <= m; ++k) {
                seq_t num1  = (seq_t)(2 * m - 2 * (k - 1));
                seq_t num2  = (seq_t)(2 * m - 2 * (k - 1) - 1);
                seq_t denom = (seq_t)(2 * k) * (2 * k - 1);
                C = (C * num1 * num2) / denom;   /* next binomial coefficient */

                sum += C * newcache[m - k];
            }

            newcache[m] = 4 * sum;  /* a(m) = 4 * sum */
        }

        /* Replace the old cache with the enlarged one */
        cache     = newcache;
        cache_size = newsize;
    }

    return cache[n];
}
