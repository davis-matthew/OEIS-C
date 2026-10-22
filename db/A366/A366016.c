/*
 * A366016  G.f. A(x) satisfies: A(x) = x * (1 + A(x))^4 / (1 - 4 * A(x)).
 * https://oeis.org/A366016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula:
   a(n) = (1/n) * Σ_{k=0..n-1} binomial(n+k-1, k) * binomial(4*n, n-k-1) * 4^k
   a(0) = 0
*/
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* lazy memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t next_compute = 0;     /* next index that must be calculated */

    /* make sure cache can hold (n+1) entries */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first two terms on first entry */
    if (next_compute == 0)
    {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        next_compute = 2;
    }

    /* compute missing terms up to n */
    for (idx_t m = next_compute; m <= n; ++m)
    {
        /* for m==0 or m==1 we already know the values */
        if (m < 2) { cache[m] = (seq_t)m; continue; }

        /* 128‑bit intermediates used for safety when seq_t is an unsigned
           integer type (fits into 64‑bit).  For arbitrary‑precision builds
           the cast is safe because the constructor from an integer exists. */
        unsigned __int128 sum   = 0;
        unsigned __int128 power4 = 1;    /* 4^k */
        unsigned __int128 B = 1;         /* binomial(m-1 , 0)   */
        unsigned __int128 C = (unsigned __int128)binom(4*m, m-1);   /* binomial(4m , m-1) */

        for (idx_t k = 0; k < m; ++k)
        {
            unsigned __int128 term = B;
            term = term * C;
            term = term * power4;
            sum += term;

            if (k + 1 < m)   /* prepare next values of B, C, power4 */
            {
                B = (B * (unsigned __int128)(m + k)) / (k + 1);
                C = (C * (unsigned __int128)(m - k - 1))
                    / (unsigned __int128)(3*m + k + 2);
                power4 *= 4;
            }
        }

        /* final division by m and store result */
        cache[m] = (seq_t)(sum / m);
    }

    next_compute = n + 1;
    return cache[n];
}
