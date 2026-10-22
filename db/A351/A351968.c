/*
 * A351968  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n-1,3*k) *
 *          a(k) * a(n-3*k-1).
 * https://oeis.org/A351968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊(n-1)/3⌋} binomial(n-1, 3k) * a(k) *
 *            a(n-3k-1) (OEIS A351968)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"

/* Compute a(n).  All results are memoized in a static array. */
seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;  /* largest index whose value is in cache */

    /* ensure there is room for indices up to n */
    ensure_cache(n, &cache, &cap);

    /* initialise a(0) the first time (value is never zero for n>0) */
    if (max_computed == 0 && cache[0] == 0)
        cache[0] = (seq_t)1;

    if (n <= max_computed)
        return cache[n];

    /* compute values for all indices > max_computed up to n */
    for (idx_t m = max_computed + 1; m <= n; ++m)
    {
        seq_t sum = 0;
        idx_t max_k = (m - 1) / 3;

        for (idx_t k = 0; k <= max_k; ++k)
        {
            idx_t t   = 3 * k;           /* 3k */
            idx_t sub = m - t - 1;       /* n - 3k - 1 */

            seq_t bin = binomial(m - 1, t);   /* choose (m-1, 3k) */
            seq_t term = bin * cache[k];
            term *= cache[sub];
            sum += term;
        }

        cache[m] = sum;
        max_computed = m;
    }

    return cache[n];
}
