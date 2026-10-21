/*
 * A279078  Maximum starting value of X such that repeated replacement of X
 *          with X-ceiling(X/8) requires n steps to reach 0.
 * https://oeis.org/A279078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)*8/7) + 1, with a(0) = 0
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence.  The array is grown as needed
 * and retains its contents across successive calls. */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Return the n-th term of OEIS A279078. */
seq_t a(idx_t n)
{
    /* Ensure the cache has been initialised. */
    if (cache == NULL)
    {
        cache_size = 1;                               /* holds a(0) */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) abort();
        cache[0] = (seq_t)0;
    }

    /* If the requested index is already computed, return it. */
    if (n < cache_size) return cache[n];

    /* Grow the cache to hold values up to a(n). */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (tmp == NULL) abort();
    cache = tmp;

    /* Fill in any newly needed entries. */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t prev = cache[i - 1];
        /* (prev * 8) / 7 performs floor division for positive numbers. */
        seq_t val = (prev * (seq_t)8) / (seq_t)7 + (seq_t)1;
        cache[i] = val;
    }
    cache_size = new_size;

    return cache[n];
}
