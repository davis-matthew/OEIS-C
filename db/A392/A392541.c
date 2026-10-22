/*
 * A392541  Expansion of 1 / ((1-x)^2 - x^6).
 * https://oeis.org/A392541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1,a(1)=2,a(2)=3,a(3)=4,a(4)=5,a(5)=6
 *            a(n)=2*a(n-1)-a(n-2)+a(n-6) for n≥6 (same as the recurrence
 *            given in OEIS)
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 58, u64 n <= 116,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* storage for computed terms */
static size_t   cap   = 0;    /* current capacity of cache */

/* Ensure that cache has space for indices up to n and that
   all terms up to n are computed. */
static void
ensure_cache(idx_t n)
{
    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = (cap == 0) ? 16 : cap << 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);              /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialize first six terms (safe to do each time) */
    if (cap > 0) {
        if (cap > 0) cache[0] = 1;
        if (cap > 1) cache[1] = 2;
        if (cap > 2) cache[2] = 3;
        if (cap > 3) cache[3] = 4;
        if (cap > 4) cache[4] = 5;
        if (cap > 5) cache[5] = 6;
    }

    /* compute terms from the first missing index up to n */
    size_t start = 6;
    if (cap < 6) start = cap;           /* if we have not yet filled >6 */
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t t1 = 2 * cache[i-1];
        seq_t t2 = t1 - cache[i-2];
        cache[i] = t2 + cache[i-6];
    }
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
