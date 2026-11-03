/*
 * A400661  a(0) = 1; a(n) = Sum_{k=1..n} (-1)^(k+1) * binomial(3*n-2*k,k) *
 *          (n-k+1)^k * a(n-k).
 * https://oeis.org/A400661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} (-1)^(k+1) * binomial(3*n-2*k, k)
 *            * (n-k+1)^k * a(n-k) for n >= 1 (OEIS offset 0)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   binomial, bit_length, ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/bit_length.h"
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* external linkage; types seq_t and idx_t are provided by the build environment */
seq_t a(idx_t n)
{
    /* static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* ensure cache has at least n+1 elements */
    if (cache_len == 0)
    {
        /* allocate for index 0 */
        cache = mem_new(seq_t, 1);
        cache[0] = 1;
        cache_len = 1;
    }

    if (n < cache_len)
        return cache[n];

    /* grow cache to include n */
    cache = mem_resize(seq_t, cache, (n + 1));
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* compute a(i) */
        seq_t pos = 0;     /* sum of terms with positive sign (k odd) */
        seq_t neg = 0;     /* sum of terms with negative sign (k even) */
        for (idx_t k = 1; k <= i; ++k)
        {
            /* binomial(3*i-2*k, k) */
            seq_t bin = binomial(3 * i - 2 * k, k);
            /* (i-k+1)^k */
            seq_t powv = ipow((seq_t)(i - k + 1), (seq_t)k);
            /* term = bin * powv * a(i-k) */
            seq_t term = bin * powv * cache[i - k];
            if (k & 1)  /* odd k -> positive sign */
                pos = pos + term;
            else        /* even k -> negative sign */
                neg = neg + term;
        }
        cache[i] = pos - neg;  /* final value; pos >= neg for this sequence */
    }
    cache_len = n + 1;
    return cache[n];
}
