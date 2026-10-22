/*
 * A352011  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} (-4)^k * a(k) *
 *          a(n-2*k-1).
 * https://oeis.org/A352011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} (-4)^k * a(k) * a(n-2*k-1),
 *            with a(0) = 1
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 20, i64 n <= 37, i128 n <= 70,
 *            big any n
 * Helpers:   alloc_new_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_new_cache.h"

/* The integer types seq_t and idx_t are provided by the build system. */
static seq_t *cache = NULL;         /* buffer holding already computed values */
static size_t cache_cap = 0;        /* allocated capacity of cache (elements)   */
static size_t cache_len = 0;        /* number of values actually computed (next to fill) */

seq_t a(idx_t n)
{
    /* Ensure the cache has enough space.  When expanding, copy the
     * already computed values to the new buffer.                      */
    if ((size_t)n + 1 > cache_cap)
    {
        seq_t *new_cache = alloc_new_cache((size_t)n + 1);
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = (size_t)n + 1;
    }

    /* Compute up to n if necessary.                                   */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t pow = (seq_t)1;                     /* (-4)^k  */
        seq_t neg4 = (seq_t)(-4);
        idx_t maxk = (i - 1) / 2;                  /* floor((i-1)/2) */

        for (idx_t k = 0; k <= maxk; ++k)
        {
            idx_t j = i - 2 * k - 1;               /* index of the second factor   */
            seq_t term = pow * cache[(size_t)k] * cache[(size_t)j];
            sum = sum + term;
            pow = pow * neg4;                     /* update (-4)^(k+1)            */
        }

        cache[(size_t)i] = sum;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
