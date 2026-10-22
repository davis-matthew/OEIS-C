/*
 * A356816  Expansion of e.g.f. exp(-x * (exp(3*x) + 1)).
 * https://oeis.org/A356816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*a(n-1) - sum_{k=1}^{n-1}
 *            binom(n-1,k)*(k+1)*3^k*a(n-1-k)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with a(0) = 1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate array for values 0..n */
    seq_t *val = (seq_t*)calloc(n + 1, sizeof(seq_t));
    if (!val) return 0;          /* In case of allocation failure */

    val[0] = (seq_t)1;

    for (idx_t i = 0; i < n; ++i)          /* compute val[i+1] from val[0..i] */
    {
        seq_t sum = 0;       /* sum_{k=1..i} binom(i,k)*(k+1)*3^k*val[i-k] */
        seq_t pow3 = 1;      /* 3^0 */
        seq_t binom = 0;     /* will hold binom(i,k) */

        for (idx_t k = 1; k <= i; ++k)
        {
            pow3 *= (seq_t)3;                  /* 3^k */

            if (k == 1)
                binom = (seq_t)i;              /* binom(i,1) = i */
            else {
                binom = binom * (seq_t)(i - k + 1);  /* binom(i,k-1)*(i-k+1) */
                binom /= (seq_t)k;                /* divide by k, exact */
            }

            seq_t term = binom * pow3 * (seq_t)(k + 1) * val[i - k];
            sum += term;
        }

        val[i + 1] = - (seq_t)2 * val[i] - sum;
    }

    seq_t result = val[n];
    free(val);
    return result;
}
