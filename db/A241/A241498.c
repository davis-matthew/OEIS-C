/*
 * A241498  q-Lucas numbers with q=2.
 * https://oeis.org/A241498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2^(n-2)*a(n-2), a(0) = 2, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of already computed terms   */
    static idx_t cache_len = 0;       /* number of terms stored (max index + 1) */

    /* Initialize the cache with the first two terms if not done yet */
    if (cache_len == 0)
    {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
    }

    /* If the requested index is already cached, return it immediately */
    if (n + 1 <= cache_len)
        return cache[n];

    /* Grow the cache to hold up to index n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    cache = tmp;
    idx_t old_len = cache_len;
    cache_len = n + 1;

    /* Compute missing terms using the recurrence relation */
    for (idx_t i = old_len; i <= n; ++i)
    {
        /* i >= 2 because old_len is at least 2 after initialization */
        seq_t term = cache[i - 1] + pow2(i - 2) * cache[i - 2];
        cache[i] = term;
    }

    return cache[n];
}
