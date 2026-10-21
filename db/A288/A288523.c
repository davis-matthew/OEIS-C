/*
 * A288523  a(n) = a(n-1) + a(n-2) + a(n-3) - 2*a(n-4) for n >= 5, where a(0)
 *          = 2, a(1) = 4, a(2) = 5, a(3) = 8, a(4) = 11.
 * https://oeis.org/A288523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - 2*a(n-4), a(0)=2, a(1)=4,
 *            a(2)=5, a(3)=8, a(4)=11, for n≥5
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 51, u64 n <= 104,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static idx_t   cap   = 0;        /* current cached size */

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cap > n)
        return;                  /* already large enough */

    /* Start with at least 5 entries for the initial values */
    if (cap < 5)
    {
        if (cache)
            free(cache);
        cache = (seq_t *)malloc(sizeof(seq_t) * 5);
        cache[0] = 2;
        cache[1] = 4;
        cache[2] = 5;
        cache[3] = 8;
        cache[4] = 11;
        cap = 5;
    }

    /* Resize if necessary */
    if (cap <= n)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        cache = new_cache;
        /* Compute new values from old cap up to n */
        for (idx_t i = cap; i <= n; ++i)
        {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            cache[i] = a1 + a2 + a3 - 2 * a4;
        }
        cap = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 5)
    {
        /* Direct return for the first five terms */
        static const seq_t init[5] = { 2, 4, 5, 8, 11 };
        return init[(idx_t)n];
    }

    ensure_cache(n);
    return cache[n];
}
