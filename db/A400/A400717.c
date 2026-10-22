/*
 * A400717  G.f. A(x) satisfies: A(x) = 1 + x * A(2*x/(1 - 2*x)^2) / (1 - x).
 * https://oeis.org/A400717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(0) = 1 a(1) = 1 a(n) = a(n-1) + 2^(n-1) * Σ_{k=1}^{n-1}
 *            binomial(n+k-2, 2*k-1) * a(k) for n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, ensure_cache, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                   /* for size_t */
#include "../utils/ensure_cache.h"    /* provides ensure_cache */
#include "../utils/binomial.h"        /* provides binomial */
#include "../utils/pow2.h"            /* provides pow2 */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;

    /* ensure we have room for indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise base cases if not already set */
    if (cache[0] == 0) cache[0] = 1;
    if (n >= 1 && cache[1] == 0) cache[1] = 1;

    if (n <= 1) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (cache[i] != 0) continue;   /* already computed */

        seq_t sum = 0;
        for (idx_t k = 1; k <= i - 1; ++k)
        {
            seq_t coef = binomial(i + k - 2, 2 * k - 1);
            sum += coef * cache[k];
        }

        seq_t term = pow2(i - 1) * sum;
        cache[i] = cache[i - 1] + term;
    }

    return cache[n];
}
