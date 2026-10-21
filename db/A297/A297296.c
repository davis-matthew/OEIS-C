/*
 * A297296  Number of n X 5 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 2 neighboring 1s.
 * https://oeis.org/A297296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3) - 2*a(n-4) for n>5 Initial
 *            terms: a(1)=1, a(2)=4, a(3)=9, a(4)=21, a(5)=49
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache to memoize computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Resize the cache to hold at least 'size' terms (a(1)..a(size)) */
static void ensure_cache(idx_t size)
{
    if (size <= cache_len)
        return;              /* already large enough */

    /* Allocate or grow the cache */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * size);
    /* realloc returns NULL only on out‑of‑memory; for competition we ignore that */
    cache = new_cache;
    cache_len = size;

    /* Initialize new entries */
    for (idx_t i = 0; i < size; ++i)
    {
        if (i == 0)                     /* a(1) */
            cache[0] = (seq_t)1;
        else if (i == 1)                /* a(2) */
            cache[1] = (seq_t)4;
        else if (i == 2)                /* a(3) */
            cache[2] = (seq_t)9;
        else if (i == 3)                /* a(4) */
            cache[3] = (seq_t)21;
        else if (i == 4)                /* a(5) */
            cache[4] = (seq_t)49;
        else
        {
            /* Recurrence: a(n) = 2*a(n-1)+2*a(n-2)-2*a(n-3)-2*a(n-4) */
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-3];
            seq_t t4 = cache[i-4];
            cache[i] = (seq_t)2 * t1 + (seq_t)2 * t2
                       - (seq_t)2 * t3 - (seq_t)2 * t4;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined for offset 1, return 0 */

    ensure_cache(n);                 /* compute up to a(n) */
    return cache[n-1];               /* cache is 0‑based: a(1) at [0] */
}
