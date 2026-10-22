/*
 * A322513  Expansion of e.g.f. log(1 + Sum_{k>=1} d(k) * x^k / k!), where
 *          d(k) = number of divisors of k (A000005).
 * https://oeis.org/A322513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0; a(1)=1; a(n)=d(n)-d(n-1)-∑_{k=1}^{n-2}
 *            binom(n-1,k)*d(k)*a(n-k) for n≥2 where d(k) = number of divisors
 *            of k. The recurrence is derived from the logarithmic transform
 *            of the e.g.f. of d(k)
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 15, i64 n <= 23, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* memoisation tables */
    static idx_t   max_n = 0;
    static seq_t *g     = NULL;   /* a(i) */
    static uint64_t *d    = NULL; /* divisor counts d(i) */

    /* return the first term directly */
    if (n == 0)
        return (seq_t)0;

    /* enlarge tables if necessary */
    if (n > max_n) {
        idx_t new_n = n;

        /* reallocate g array */
        g = (seq_t *)realloc(g, (new_n + 1) * sizeof(seq_t));
        /* reallocate d array (divisor counts) */
        d = (uint64_t *)realloc(d, (new_n + 1) * sizeof(uint64_t));
        if (!g || !d) return (seq_t)0; /* allocation failure */

        /* initialise divisor counts up to new_n */
        for (idx_t i = 0; i <= new_n; ++i)
            d[i] = 0;
        for (idx_t i = 1; i <= new_n; ++i) {
            for (idx_t j = i; j <= new_n; j += i)
                ++d[j];
        }

        /* compute a(i) for i from old max+1 up to new_n */
        for (idx_t i = 0; i <= new_n; ++i) g[i] = 0;  /* safety clear */

        g[0] = (seq_t)0;
        if (new_n >= 1) g[1] = (seq_t)1;

        /* recurrence for i ≥ 2 */
        for (idx_t i = 2; i <= new_n; ++i) {
            seq_t sum = 0;
            seq_t bin = 1;   /* C(i-1,0) */

            for (idx_t k = 1; k <= i - 2; ++k) {
                /* compute binomial coefficient C(i-1,k) iteratively */
                bin = bin * (seq_t)(i - k) / (seq_t)k;
                sum += bin * (seq_t)d[k] * g[i - k];
            }
            g[i] = (seq_t)d[i] - (seq_t)d[i - 1] - sum;
        }

        max_n = new_n;
    }

    return g[n];
}
