/*
 * A349842  Expansion of 1/((1 - 2*x)*(1 + x + x^2 + x^3 + x^4)).
 * https://oeis.org/A349842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A349842
   Recurrence: a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+2*a(n-5)   (a(k<0)=0, a(0)=1) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    /* initialise cache for a(0) */
    if (cap == 0)
    {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cap = 1;
    }

    if (n < cap)          /* already computed */
        return cache[n];

    /* expand cache up to n */
    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* compute missing terms */
    for (idx_t i = cap; i < new_cap; ++i)
    {
        seq_t s = 0;
        if (i >= 1) s += cache[i - 1];
        if (i >= 2) s += cache[i - 2];
        if (i >= 3) s += cache[i - 3];
        if (i >= 4) s += cache[i - 4];
        if (i >= 5) s += cache[i - 5] * (seq_t)2;
        cache[i] = s;
    }

    cap = new_cap;
    return cache[n];
}
