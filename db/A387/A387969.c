/*
 * A387969  Expansion of e.g.f. (1+x) * exp(x^2*(1+x)^2).
 * https://oeis.org/A387969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*(n-1)*a(n-2) + 2*(n-2)*(3*n-2)*a(n-3)
 *               + 4*(n-3)*(n-2)*n*a(n-4)
 *  with initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=18. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small table for the base cases */
    static const seq_t base[4] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)18 };

    /* Static cache for memoized values */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* number of entries currently stored */

    /* Return directly for small indices */
    if (n < 4) {
        return base[(size_t)n];
    }

    size_t old_cap = cap;
    if (old_cap <= (size_t)n) {
        /* Enlarge the cache */
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 4) new_cap = 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {               /* allocation failure – not expected */
            return (seq_t)0;
        }
        cache = tmp;
        /* Initialize the base values once */
        if (old_cap == 0) {
            cache[0] = base[0];
            cache[1] = base[1];
            cache[2] = base[2];
            cache[3] = base[3];
        }
        cap = new_cap;
    }

    /* Compute any missing values up to n */
    size_t start = (old_cap < 4) ? 4 : old_cap;
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t term1 = (seq_t)2 * (seq_t)(i - 1) * cache[i - 2];
        seq_t term2 = (seq_t)2 * (seq_t)(i - 2)
                      * (seq_t)(3 * i - 2)
                      * cache[i - 3];
        seq_t term3 = (seq_t)4 * (seq_t)(i - 3) * (seq_t)(i - 2)
                      * (seq_t)i * cache[i - 4];
        cache[i] = term1 + term2 + term3;
    }

    return cache[(size_t)n];
}
