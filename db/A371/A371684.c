/*
 * A371684  a(n) = Sum_{k=0..n} 2^(3*k)*binomial(2*n, 2*k)*Euler(2*k, 1/2).
 *          Alternating row sums of A371637.
 * https://oeis.org/A371684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 – Σ_{k=0..n‑1} 2^(n‑k)·binomial(2n,2k)·a(k) (recurrence
 *            with a(0) = 1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for the OEIS sequence A371684 */
seq_t a(idx_t n)
{
    /* static table of already computed values */
    static seq_t *a_cache = NULL;   /* a_cache[0..m] */
    static idx_t a_size   = 0;      /* current size of a_cache   */

    /* static table of powers of two: pow2_cache[i] = 2^i */
    static seq_t *pow2_cache = NULL;  /* pow2_cache[0..m] */
    static idx_t pow2_size   = 0;     /* current size of pow2_cache */

    /* initialise for the first call */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        a_cache[0] = (seq_t)1;      /* a(0) = 1 */
        a_size   = 1;
    }

    if (pow2_cache == NULL) {
        pow2_cache = (seq_t *)malloc(sizeof(seq_t));
        pow2_cache[0] = (seq_t)1;    /* 2^0 = 1 */
        pow2_size = 1;
    }

    /* grow the tables if necessary */
    if (n + 1 > a_size) {
        /* resize a_cache to hold values up to n */
        a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));

        /* resize pow2_cache to at least n+1 entries */
        if (n + 1 > pow2_size) {
            pow2_cache = (seq_t *)realloc(pow2_cache, (n + 1) * sizeof(seq_t));
            for (idx_t i = pow2_size; i <= n; ++i)
                pow2_cache[i] = pow2_cache[i - 1] * (seq_t)2;
            pow2_size = n + 1;
        }

        /* compute a values from a_size up to n */
        for (idx_t m = a_size; m <= n; ++m) {
            if (m == 0) {                /* protection, although a_cache[0] is already set */
                a_cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum   = 0;
            seq_t binom = 1;              /* binomial(2m,0) */
            seq_t factor = pow2_cache[m]; /* 2^m  (== 2^(m-0)) */

            for (idx_t k = 0; k < m; ++k) {
                /* term = a(k) * binom * 2^(m-k) */
                seq_t term = a_cache[k];
                term *= binom;
                term *= factor;
                sum += term;

                /* update binomial for next k */
                idx_t num1 = 2 * m - 2 * k;     /* 2m-2k   */
                idx_t num2 = 2 * m - 2 * k - 1; /* 2m-2k-1 */
                idx_t den1 = 2 * k + 2;         /* 2k+2    */
                idx_t den2 = 2 * k + 1;         /* 2k+1    */

                seq_t tmp = binom;
                tmp *= (seq_t)num1;
                tmp *= (seq_t)num2;
                binom = tmp / ((seq_t)den1 * (seq_t)den2);

                /* factor becomes 2^(m-(k+1)) */
                factor /= (seq_t)2;
            }

            a_cache[m] = (seq_t)1 - sum;
        }

        a_size = n + 1;
    }

    return a_cache[n];
}
