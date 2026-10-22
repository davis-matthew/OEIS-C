/*
 * A377946  Cogrowth sequence of the 16-element group C2^2:C4 = <S,T | S^4,
 *          T^2, (ST)^2, (S^2T)^2>.
 * https://oeis.org/A377946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=10, a(4)=40; for n≥5:
 *            a(n)=6*a(n-1)-12*a(n-2)+24*a(n-3)-32*a(n-4)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values */
static idx_t cache_size = 0;     /* number of computed entries */
static size_t cache_cap = 0;     /* allocated capacity */

/* Ensure the cache can hold values up to index n (inclusive) */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    if (!cache)
    {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    }
    else
    {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
    }
    cache_cap = new_cap;
}

/* Cogrowth sequence A377946 */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* initialise first 5 terms if not already done */
    if (cache_size == 0)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)40;
        cache_size = 5;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i < 5) continue;         /* already initialised */

        /* a(i) = 6*a(i-1) - 12*a(i-2) + 24*a(i-3) - 32*a(i-4) */
        seq_t t = cache[i - 1] * (seq_t)6;
        t -= cache[i - 2] * (seq_t)12;
        t += cache[i - 3] * (seq_t)24;
        t -= cache[i - 4] * (seq_t)32;
        cache[i] = t;
    }
    cache_size = n + 1;

    return cache[n];
}
