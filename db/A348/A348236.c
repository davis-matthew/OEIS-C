/*
 * A348236  Coordination sequence for the cpq net with respect to a node where
 *          a square, hexagon, and octagon meet.
 * https://oeis.org/A348236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the rational generating function G.f. =
 *            (1+q)^2*(q^6+2*q^4+q^3+2*q^2+1) / ((1-q)*(1+q^2)*(1-q^5)) which
 *            expands to the linear recurrence a(n) = a(n-1) - a(n-2) + a(n-3)
 *            + a(n-5) - a(n-6) + a(n-7) - a(n-8) for n ≥ 9, with initial
 *            values a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=12, a(5)=...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Coordination sequence for the cpq net.  The function is memoized
   so that repeated calls are fast, no matter the order of indices. */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache   = NULL;  /* array of computed terms      */
    static size_t  cap    = 0;      /* allocated size of 'cache'   */
    static size_t  len    = 0;      /* number of terms currently computed */

    /* initialise cache with first 9 terms if needed */
    if (cache == NULL) {
        cap = 9;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)12;
        cache[5] = (seq_t)15;
        cache[6] = (seq_t)16;
        cache[7] = (seq_t)18;
        cache[8] = (seq_t)23;
        len = 9; /* terms a[0]..a[8] are ready */
    }

    size_t idx = (size_t)n;

    /* if requested term already computed, return it */
    if (idx < len) {
        return cache[idx];
    }

    /* extend allocation if required */
    if (idx + 1 > cap) {
        size_t new_cap = idx + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < len; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing terms using the linear recurrence */
    for (size_t i = len; i <= idx; ++i) {
        /* a(i) = a(i-1) - a(i-2) + a(i-3) + a(i-5)
                 - a(i-6) + a(i-7) - a(i-8) */
        cache[i] = cache[i-1] - cache[i-2] + cache[i-3]
                 + cache[i-5] - cache[i-6] + cache[i-7] - cache[i-8];
    }

    /* update cache length */
    len = idx + 1;
    return cache[idx];
}
