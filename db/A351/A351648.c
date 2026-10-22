/*
 * A351648  G.f. A(x) satisfies: A(x) = 1 + x + x^2 * A(x/(1 - x)) / (1 -
 *          x)^5.
 * https://oeis.org/A351648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; for n >= 2, a(n) = Sum_{k=0..n-2} binomial(n+2,
 *            k+4) * a(k)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom_start
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom_start.h"

/* Memoisation cache */
static seq_t *cache = NULL;        /* array of computed values           */
static idx_t cache_cap = 0;        /* allocated number of slots          */
static idx_t cache_count = 0;      /* number of values already computed  */

/* Grow the cache so that it can hold indices 0..n inclusive */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = cache_cap == 0 ? 8 : cache_cap << 1;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Allocate enough space for index n   */
    ensure_capacity(n);

    /* Compute missing terms up to n    */
    for (idx_t m = cache_count; m <= n; ++m)
    {
        if (m == 0 || m == 1)
        {
            cache[m] = (seq_t)1;
            continue;
        }

        seq_t sum = 0;
        seq_t bin = binom_start(m);     /* C(m+2,4)   */

        for (idx_t k = 0; k <= m - 2; ++k)
        {
            sum += bin * cache[k];

            /* Prepare bin for next k   */
            if (k < m - 2)
            {
                /* C(m+2, k+5) = C(m+2, k+4) * (m - k - 2) / (k + 5)   */
                seq_t num = (seq_t)(m - k - 2);
                seq_t den = (seq_t)(k + 5);
                bin = bin * num / den;
            }
        }
        cache[m] = sum;
    }
    cache_count = n + 1;
    return cache[n];
}
