/*
 * A368018  Expansion of e.g.f. exp(exp(2*x) - exp(3*x)).
 * https://oeis.org/A368018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=Sum_{k=1..n} (2^k-3^k)*binomial(n-1,k-1)*a(n-k) */
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Initialize cache if needed */
    if (cache == NULL)
    {
        cache = seq_alloc_new_array(1);
        cache[0] = 1;             /* a(0) = 1 */
        cache_cap = 1;
    }

    size_t nsize = (size_t)n;
    if (nsize >= cache_cap)
    {
        /* Allocate a larger array that can hold up to a(n) */
        seq_t *new_cache = seq_alloc_new_array(nsize + 1);

        /* Copy existing terms */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];

        /* Compute missing terms */
        for (idx_t i = (idx_t)cache_cap; i <= n; ++i)
        {
            seq_t sum = 0;
            seq_t C    = 1;      /* binomial(i-1,0)  */
            seq_t pow2 = 2;      /* 2^1 */
            seq_t pow3 = 3;      /* 3^1 */

            for (idx_t k = 1; k <= i; ++k)
            {
                seq_t coeff = pow2 - pow3;                 /* (2^k - 3^k) */
                seq_t term  = coeff * C * new_cache[(size_t)(i - k)];
                sum += term;

                if (k < i)
                {
                    /* Advance binomial coefficient to C = binomial(i-1,k) */
                    C = (C * (i - k)) / k;
                    /* Advance powers for the next k */
                    pow2 *= 2;
                    pow3 *= 3;
                }
            }
            new_cache[(size_t)i] = sum;
        }

        /* Free old cache and update state */
        seq_free_array(cache);
        cache = new_cache;
        cache_cap = nsize + 1;
    }

    return cache[(size_t)n];
}
