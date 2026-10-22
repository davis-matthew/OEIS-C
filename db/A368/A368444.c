/*
 * A368444  Expansion of e.g.f. exp(x) / (1 + log(1 - 2*x)).
 * https://oeis.org/A368444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} 2^k * (k-1)! * binomial(n,k) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*       a(0) = 1   */

#include <stdlib.h>   /* for realloc */

seq_t a(idx_t n) {
    /* Static memoisation tables */
    static seq_t *seq  = NULL;   /* a(0) .. a(current_len-1) */
    static seq_t *coef = NULL;   /* coef[k] = 2^k * (k-1)!   */
    static idx_t   len  = 0;     /* number of terms currently stored */

    /* Grow tables if needed */
    if (len <= n) {

        /* Allocate (or grow) the a table */
        seq  = (seq_t *)realloc(seq,  sizeof(seq_t)  * (n + 1));
        /* Allocate (or grow) the coefficient table */
        coef = (seq_t *)realloc(coef, sizeof(seq_t) * (n + 1));

        /* Compute coefficients up to n (recompute all if small n) */
        if (n >= 1) {
            coef[1] = (seq_t)2;                      /* 2^1 * 0! = 2 */
            for (idx_t k = 2; k <= n; ++k)
                /* coef[k] = coef[k-1] * 2 * (k-1) */
                coef[k] = coef[k - 1] * (seq_t)2 * (seq_t)(k - 1);
        }

        /* Compute a(i) for all new indices i = len .. n */
        /* Note: a(0) is already known; set it if we are starting from scratch. */
        if (len == 0) {
            seq[0] = (seq_t)1;   /* a(0) = 1 */
            len    = 1;
        }

        for (idx_t i = len; i <= n; ++i) {
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)i;   /* binomial(i, 1) = i */

            for (idx_t k = 1; k <= i; ++k) {
                /* term = coef[k] * a(i-k) * binom */
                seq_t term = coef[k];
                term = term * seq[i - k];
                term = term * binom;
                sum += term;

                /* Update binomial coefficient for next k
                   binom = C(i, k+1) = C(i, k) * (i-k) / (k+1) */
                if (k < i) {
                    binom = binom * (seq_t)(i - k);
                    binom = binom / (seq_t)(k + 1);
                }
            }
            seq[i] = sum + (seq_t)1;
        }

        len = n + 1;   /* update stored length */
    }

    return seq[n];
}
