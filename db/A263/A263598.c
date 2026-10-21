/*
 * A263598  Number of (n+1) X (1+1) arrays of permutations of 0..n*2+1 filled
 *          by rows with each element moved a city block distance of 0 or 2,
 *          and rows and columns in increasing lexicographic order.
 * https://oeis.org/A263598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-3) + 4*a(n-4) a(0) = 1, a(1) = 2, a(2) =
 *            8, a(3) = 24
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Extend the cache up to index n (inclusive). */
static void extend_cache(idx_t n)
{
    if (n < cache_size)
        return;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
    {
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)2;
        else if (i == 2)
            cache[i] = (seq_t)8;
        else if (i == 3)
            cache[i] = (seq_t)24;
        else
            cache[i] = 2 * cache[i - 1] + 4 * cache[i - 3] + 4 * cache[i - 4];
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}
