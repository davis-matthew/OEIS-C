/*
 * A361817  Expansion of 1/sqrt(1 - 4*x*(1-x)^4).
 * https://oeis.org/A361817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 17, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A361817: 1/sqrt(1 - 4*x*(1-x)^4)
    Formula: a(n) = Σ_{k=0..n} (-1)^(n-k) * binomial(2*k,k) * binomial(4*k,n-k)
    Recurrence (for n>4):
          n*a(n) = 2 * ( (2n-1)*a(n-1)
                      - 4*(2n-2)*a(n-2)
                      + 6*(2n-3)*a(n-3)
                      - 4*(2n-4)*a(n-4)
                      + (2n-5)*a(n-5) )
    Initial terms:
          a(0)=1, a(1)=2, a(2)=-2, a(3)=-16, a(4)=-10
    The function computes a(n) for any n≥0, caching previously
    computed values for efficiency.  It uses the provided
    ensure_cache helper to grow the array as needed.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"   /* provided by build environment */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;  /* array of seq_t values  */
    static size_t cache_cap = 0;   /* allocated capacity       */
    static size_t cache_len = 0;   /* number of computed entries */

    /* first call: initialise cache with indices 0..4 */
    if (!cache)
    {
        /* ensure at least 5 elements (indices 0..4) */
        ensure_cache(4, &cache, &cache_cap);

        cache[0] =  1;
        cache[1] =  2;
        cache[2] = -2;
        cache[3] = -16;
        cache[4] = -10;
        cache_len = 5;          /* next free index is 5 */
    }

    /* if *n* is already computed, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* need to compute up to n inclusive */
    ensure_cache(n, &cache, &cache_cap);

    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t ai;
        if (i < 5)
        {
            /* already initialised, but defensive copy */
            ai = cache[i];
        }
        else
        {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];

            /* build the right‑hand side of the recurrence */
            seq_t term =
                (seq_t)(2) *
                ( (seq_t)(2*i-1) * a1
                  - (seq_t)(4*(2*i-2)) * a2
                  + (seq_t)(6*(2*i-3)) * a3
                  - (seq_t)(4*(2*i-4)) * a4
                  + (seq_t)(2*i-5) * a5 );

            /* divide by i to get a(i) */
            ai = term / (seq_t)i;
        }
        cache[i] = ai;
    }

    cache_len = n + 1;
    return cache[n];
}
