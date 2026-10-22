/*
 * A337016  a(0) = 0. Successive terms are double the previous, then with
 *          their digits incremented by 1.
 * https://oeis.org/A337016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 16, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   increment_and_double
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; for n>0, a(n) = A216556(2*a(n-1)) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/increment_and_double.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with a(0) */
    if (cache_size == 0)
    {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            return 0;                           /* allocation failed */
        cache[0] = 0;
        cache_size = 1;
    }

    /* fill cache up to index n if needed */
    if ((idx_t)n >= cache_size)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0;                           /* allocation failed */
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = increment_and_double(cache[i - 1]);
        cache_size = new_cap;
    }

    return cache[n];
}
