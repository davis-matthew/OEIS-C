/*
 * A276057  Sum of the asymmetry degrees of all compositions of n with parts
 *          in {1,3}.
 * https://oeis.org/A276057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 54, u64 n <= 110,
 *            u128 n <= 224, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276057: sum of the asymmetry degrees of all compositions of n
 * with parts in {1,3}.
 *
 * The ordinary generating function is
 *     G(z) = 2*z^4 / ((1+z+z^3)*(1-z-z^3)^2)
 *
 * Let D(z) = (1+z+z^3)*(1-z-z^3)^2.
 *   D(z) = 1 - z - z^2 - 2*z^4 + 3*z^5 - z^6 + 3*z^7 + z^9
 *
 * Since D(0)=1, the coefficients a(n) satisfy the linear recurrence
 *
 *     a(n) = a(n-1) + a(n-2) + 2*a(n-4) - 3*a(n-5)
 *            + a(n-6) - 3*a(n-7) - a(n-9)        for n ≥ 9
 *
 * with the following initial values:
 *     a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=2,
 *     a(6)=4, a(7)=6, a(8)=14.
 *
 * The implementation below memoises computed terms and extends the cache
 * as needed, using only integer arithmetic allowed by the problem
 * constraints.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;        /* allocated capacity */
    static idx_t    upto  = (idx_t)-1;/* last computed index */

    /* Initial values for n = 0..8 */
    static const seq_t init[9] = {
        0, 0, 0, 0, 2, 2, 4, 6, 14
    };

    /* First allocation and initialisation */
    if (cache == NULL) {
        cap = 9;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();      /* allocation failed */

        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];

        upto = 8;                         /* terms 0..8 known */
    }

    /* Ensure the cache can hold up to index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n */
    if (upto < n) {
        for (size_t i = (size_t)upto + 1; i <= (size_t)n; ++i) {
            /* For i <= 8 we simply copy the initial values */
            if (i < 9) {
                cache[i] = init[i];
                continue;
            }

            /* Recurrence:
             * a(i) = a(i-1) + a(i-2) + 2*a(i-4) - 3*a(i-5)
             *        + a(i-6) - 3*a(i-7) - a(i-9)
             */
            seq_t pos = cache[i-1] + cache[i-2];
            pos += cache[i-4] * 2;
            pos += cache[i-6];

            seq_t neg = cache[i-5] * 3;
            neg += cache[i-7] * 3;
            neg += cache[i-9];

            cache[i] = pos - neg;
        }
        upto = n;
    }

    return cache[(size_t)n];
}
