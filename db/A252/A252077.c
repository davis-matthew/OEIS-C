/*
 * A252077  Numbers n such that the hexagonal number X(n) is equal to the sum
 *          of the heptagonal number H(m) and H(m+1) for some m.
 * https://oeis.org/A252077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1443*a(n-1)-1443*a(n-2)+a(n-3); Initial values: a(1)=1,
 *            a(2)=769, a(3)=1108537
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

/* Ensure cache has at least *needed* capacity. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t newCap = cache_cap ? cache_cap * 2 : 4;
    while (newCap < needed)
        newCap *= 2;
    cache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
    cache_cap = newCap;
}

/* Initialise the first three terms if they are not yet present. */
static void init_cache(void)
{
    if (cache_size >= 3)
        return;
    grow_cache(3);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)769;
    cache[2] = (seq_t)1108537;
    cache_size = 3;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. */
    init_cache();

    if (n <= cache_size)
        return cache[n - 1];

    /* Compute missing terms up to n. */
    seq_t c = (seq_t)1443;
    size_t i;
    grow_cache((size_t)n);
    for (i = cache_size; i < (size_t)n; ++i)
    {
        cache[i] = c * cache[i - 1] - c * cache[i - 2] + cache[i - 3];
    }
    cache_size = (size_t)n;
    return cache[n - 1];
}
