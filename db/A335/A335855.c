/*
 * A335855  a(n) = a(a(n-1)) + a(floor(n/a(n-1))) with a(1) = a(2) = a(3) =
 *          a(4) = 2.
 * https://oeis.org/A335855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(a(n-1)) + a(floor(n / a(n-1))), a(1)=a(2)=a(3)=a(4)=2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values: indices 1..last */
    static seq_t *cache = NULL;     /* array holding values of a(i) */
    static idx_t cap = 0;           /* allocated capacity */
    static idx_t last = 0;          /* largest index computed so far */

    /* base values */
    if (n <= 4)
        return 2;

    /* initialise cache on first call */
    if (!cache) {
        cap = 16;  /* initial capacity */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache)
            return 0; /* memory allocation failure – return 0 as sentinel */
        cache[1] = cache[2] = cache[3] = cache[4] = 2;
        last = 4;
    }

    /* grow capacity if needed */
    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;          /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */

        cache = tmp;
        /* new slots are uninitialized; we will fill them */
        cap = new_cap;
    }

    /* compute any missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 4) {          /* should not happen after init, but be safe */
            cache[i] = 2;
            continue;
        }

        seq_t a_prev = cache[i - 1];
        idx_t denom = (idx_t)a_prev;        /* a(i-1) used as divisor */
        idx_t idx2   = i / denom;           /* floor(i / a(i-1))   */

        /* a(a(i-1)) -> cache[denom]   */
        seq_t term1 = cache[denom];

        /* a(floor(i / a(i-1))) -> cache[idx2]   */
        seq_t term2 = cache[idx2];

        cache[i] = term1 + term2;
    }

    last = (last < n) ? n : last;
    return cache[(idx_t)n];
}
