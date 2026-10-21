/*
 * A296543  Expansion of e.g.f. tanh(exp(x)-1).
 * https://oeis.org/A296543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0 For n>=1, a(n) = 1 - Σ_{p=1}^{n-1} binom(n-1,p) * c(p)
 *            where c(0)=0 and for k>=0 c(k) = Σ_{i=0}^{k} binom(k,i) * a(i) *
 *            a(k-i) (All binomial coefficients are ordinary integers.)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build. */

seq_t a(idx_t n)
{
    /* Static arrays to memoise the sequence. */
    static idx_t cache_size = 0;          /* number of elements already stored   */
    static seq_t *a_vals = NULL;          /* a_vals[0..cache_size-1]             */
    static seq_t *c_vals = NULL;          /* c_vals[0..cache_size-1]             */

    /* Grow the cache if necessary. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;           /* allocate up to n inclusive           */
        a_vals = (seq_t *)realloc(a_vals, new_size * sizeof(seq_t));
        c_vals = (seq_t *)realloc(c_vals, new_size * sizeof(seq_t));

        /* Initialise the first element.  For n==0 we always have a(0)=0.  */
        if (cache_size == 0) {
            a_vals[0] = (seq_t)0;
            c_vals[0] = (seq_t)0;        /* a(0)^2 = 0                           */
            cache_size = 1;
        }

        /* Compute the sequence up to the requested n.  */
        for (idx_t m = cache_size; m <= n; ++m) {
            seq_t res = (seq_t)1;          /* start with the p=0 term (binom=1)   */

            /* --- Compute a(m)  ---------------------------------------- */
            seq_t binom = (seq_t)1;        /* binom(m-1,0)                        */
            for (idx_t p = 1; p <= m-1; ++p) {
                /* binom(m-1,p) = binom * (m-p) / p */
                binom = binom * (seq_t)(m - p) / (seq_t)p;
                res -= binom * c_vals[p];
            }
            a_vals[m] = res;

            /* --- Compute c(m)  ---------------------------------------- */
            seq_t csum = (seq_t)0;
            binom = (seq_t)1;              /* binom(m,0)                          */
            for (idx_t i = 0; i <= m; ++i) {
                seq_t prod = a_vals[i] * a_vals[m - i];
                csum += binom * prod;
                if (i < m) {
                    binom = binom * (seq_t)(m - i) / (seq_t)(i + 1);
                }
            }
            c_vals[m] = csum;
        }
        cache_size = new_size;
    }

    return a_vals[n];
}
