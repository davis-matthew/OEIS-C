/*
 * A335025  Largest side lengths of almost-equilateral Heronian triangles.
 * https://oeis.org/A335025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2+sqrt(3))^n + (2-sqrt(3))^n + 1 Recurrence: a(n) =
 *            5*a(n-1) - 5*a(n-2) + a(n-3) for n >= 4 with initial values
 *            a(1)=5, a(2)=15, a(3)=53
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1u)                /* offset is 1, behaviour for n<1 not required */
        return (seq_t)0;

    /* 1‑based cache: cache[0] is unused */
    static seq_t *cache = NULL;    /* pointer to allocated array */
    static idx_t  cached = 0;      /* largest n for which cache[n] is valid */
    static size_t cap    = 0;      /* allocated array size (>= cached+1) */

    /* Resize array if necessary */
    if (cap <= n) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise first three terms once */
    if (cached == 0) {
        cache[0] = (seq_t)0;   /* unused */
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)53;
        cached   = 3;
    }

    /* Return cached value if already computed */
    if (n <= cached)
        return cache[n];

    /* Compute terms from cached+1 up to n, starting from n = 4 */
    idx_t start = cached + 1;
    if (start < (idx_t)4) start = (idx_t)4;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i - 1] - (seq_t)5 * cache[i - 2] + cache[i - 3];
    }
    cached = n;
    return cache[n];
}
