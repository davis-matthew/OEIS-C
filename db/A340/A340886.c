/*
 * A340886  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(n,k)^2 * 2^(n-k-1) *
 *          a(k).
 * https://oeis.org/A340886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=∑_{k=0}^{n-1} binomial(n,k)^2 * 2^{n-k-1} * a(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   ensure_cache, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/pow2.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_up_to = 0u;

seq_t a(idx_t n)
{
    /* Ensure the cache can hold index n */
    if (cache == NULL || n >= (idx_t)cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    /* Initialise a(0) if it has not yet been set */
    if (cached_up_to == 0 && cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;

    /* Compute any missing terms up to n */
    for (idx_t m = cached_up_to + 1; m <= n; ++m)
    {
        seq_t sum = 0;
        seq_t c = 1;                     /* binomial(m,0) */
        seq_t p = pow2(m - 1);           /* 2^(m-1) */

        for (idx_t k = 0; k < m; ++k)
        {
            seq_t term = c;
            term = term * c;             /* c^2 */
            term = term * p;             /* * 2^(m-k-1) */
            term = term * cache[k];      /* * a(k) */
            sum += term;

            /* Update binomial coefficient to binomial(m,k+1) */
            seq_t mult = c * (seq_t)(m - k);
            c = mult / ( (seq_t)(k + 1) );

            /* Update power of two: divide by 2 */
            p = p / ( (seq_t)2 );
        }

        cache[m] = sum;
    }

    cached_up_to = n;
    return cache[n];
}
