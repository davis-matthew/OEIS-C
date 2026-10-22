/*
 * A329725  a(1)=0, a(n) = n - (product of nonzero digits of n) - a(n-1).
 * https://oeis.org/A329725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(1) = 0, a(n) = n - (product of nonzero digits of n) - a(n-1)
 * Range:     i8 n <= 79, i16 n <= 2959, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   nonzero_digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/nonzero_digit_product.h"

static seq_t *cache = NULL;          /* cached values    */
static idx_t cache_capacity = 0;     /* number of cached elements */

seq_t a(idx_t n)
{
    /* In strict OEIS sense n ≥ 1.  Return 0 for n == 0 for safety. */
    if (n == 0) return 0;

    /* If the cache is too small, extend it and fill new entries. */
    if (n > cache_capacity)
    {
        /* Allocate a larger buffer, keeping existing values. */
        seq_t *new_cache = (seq_t *)malloc(n * sizeof(seq_t));
        if (new_cache == NULL)
        {
            /* Allocation failed – fall back to direct computation. */
            seq_t prod = nonzero_digit_product(n);
            seq_t prev = (n == 1) ? 0 : a(n-1);
            return (seq_t)n - prod - prev;
        }

        /* Copy old values into the new buffer. */
        for (idx_t i = 0; i < cache_capacity; ++i)
            new_cache[i] = cache[i];

        free(cache);
        cache = new_cache;

        /* Compute the sequence from the first missing index up to n. */
        for (idx_t i = cache_capacity + 1; i <= n; ++i)
        {
            seq_t prod = nonzero_digit_product(i);
            seq_t prev = (i == 1) ? 0 : cache[i-2];
            cache[i-1] = (seq_t)i - prod - prev;
        }
        cache_capacity = n;
    }

    return cache[n-1];
}
