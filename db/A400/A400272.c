/*
 * A400272  G.f. A(x) satisfies A(x) = (1 + x) * A(x^4 * (1 + x)^2), with A(0)
 *          = 1.
 * https://oeis.org/A400272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(2*k+1, n-4*k) * a(k), a(0)=1
 * Range:     u8 n <= 19, u16 n <= 67, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache: cache[i] = a(i) for 0 <= i < len */
    static seq_t *cache = NULL;
    static size_t cap = 0;         /* capacity of the cache array */
    static size_t len = 0;         /* number of already computed entries */

    /* make sure the cache can hold indices up to n */
    if (n >= cap)
        ensure_cache(n, &cache, &cap);

    /* compute any missing entries up to n */
    for (; len <= n; ++len)
    {
        if (len == 0)
        {
            cache[0] = 1;
            continue;
        }

        seq_t sum = 0;

        /* Iterate over k = 0 .. floor(len/4) */
        for (idx_t k = 0; k <= len / 4; ++k)
        {
            idx_t j = len - 4 * k;          /* n - 4*k */
            /* binomial is zero if j > 2*k+1; skip those */
            if (j > 2 * k + 1)
                continue;

            seq_t term = binomial(2 * k + 1, j);
            term *= cache[k];
            sum += term;
        }

        cache[len] = sum;
    }

    return cache[n];
}
