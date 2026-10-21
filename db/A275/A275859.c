/*
 * A275859  a(n) = floor(c*s*a(n-1)) + floor(d*r*a(n-2)), where r = 1 +
 *          sqrt(2), s = r/(r-1), c = 2, d = 1, a(0) = 1, a(1) = 1.
 * https://oeis.org/A275859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n‑1) – 4*a(n‑2) + a(n‑4) – a(n‑5) Initial values:
 *            a(0)=1, a(1)=1, a(2)=5, a(3)=19, a(4)=76
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t len = 0;          /* number of elements in cache */
    static idx_t cap = 0;          /* allocated capacity */

    /* Initialise cache with the first five terms */
    if (cache == NULL) {
        /* allocate space for at least 5 items; use a small grown strategy */
        cap = 5;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed; fall back */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)19;
        cache[4] = (seq_t)76;
        len  = 5;
    }

    /* Grow cache if necessary */
    if ((idx_t)n >= cap) {
        idx_t new_cap = cap;
        while ((idx_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = len; i <= (idx_t)n; ++i) {
        seq_t val;
        if (i < 5) {
            /* already initialised */
            continue;
        } else {
            /* recurrence: a(n) = 5*a(n-1) - 4*a(n-2) + a(n-4) - a(n-5) */
            seq_t t1 = 5 * cache[i - 1];
            seq_t t2 = 4 * cache[i - 2];
            seq_t t3 = cache[i - 4];
            seq_t t4 = cache[i - 5];
            val = t1 - t2 + t3 - t4;
        }
        cache[i] = val;
    }
    len = n + 1;

    return cache[n];
}
