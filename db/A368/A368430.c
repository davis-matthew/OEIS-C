/*
 * A368430  Number of binary words of length n not containing the substrings
 *          0000, 0001, 0011, 0111.
 * https://oeis.org/A368430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - 2*a(n-4) initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=8
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 50, u64 n <= 103,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t last_computed = 3;   /* we will store a(0)..a(3) from start */

seq_t a(idx_t n)
{
    /* ensure we have the first four values */
    if (cache == NULL)
    {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
    }

    /* If we need a value that we have already computed, just return it */
    if (n <= last_computed)
        return cache[n];

    /* Grow the cache if necessary */
    if ((idx_t)(n + 1) > cache_cap)
    {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new values incrementally */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3] - ((seq_t)2) * cache[i - 4];
    }
    last_computed = n;

    return cache[n];
}
