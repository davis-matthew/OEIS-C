/*
 * A363258  Expansion of Sum_{k>0} k * x^(2*k-1) / (1 - x^(4*k-3)).
 * https://oeis.org/A363258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * Σ_{d | (4*n-2), d≡1 (mod 4)} (d+3) for n ≥ 1
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   term_for_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation iterates over all divisors of 4*n-2 by checking up
   to its square root.  The result is cached so repeated calls are fast. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/term_for_n.h"

/* Memoized scalar sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1; n==0 should never be queried */

    /* static cache ------------------------------------------------- */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated elements */
    static size_t cache_len = 0;   /* number of filled entries */

    /* grow cache if necessary */
    if ((size_t)n > cache_len)
    {
        size_t need = (size_t)n;
        if (need > cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < need)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) /* allocation failure – abort */
                return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        for (size_t i = cache_len; i < need; ++i)
        {
            cache[i] = term_for_n((idx_t)(i + 1));
        }
        cache_len = need;
    }

    return cache[n - 1];
}
