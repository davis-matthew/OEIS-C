/*
 * A254381  a(n) = 3^n*(2*n + 1)!/n!.
 * https://oeis.org/A254381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = 6*(2*n + 1)*a(n-1)   (n >= 1) */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Resize the cache if we don't have enough space yet */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)            /* allocation failure – signal error */
            return 0;
        cache = tmp;

        /* Initialise first element if this is the first allocation */
        size_t start = cache_len;
        if (cache_len == 0) {
            cache[0] = 1;   /* a(0) = 1 */
            start = 1;      /* next term to compute */
        }

        /* Compute the new terms using the recurrence */
        for (size_t i = start; i <= (size_t)n; ++i) {
            unsigned int mul = (unsigned int)(6u * (2u * (unsigned int)i + 1u));
            cache[i] = mul_small(cache[i - 1], mul);
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
