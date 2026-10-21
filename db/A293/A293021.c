/*
 * A293021  a(n) = n! * [x^n] exp(n*sin(x)).
 * https://oeis.org/A293021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * [x^n] exp(n*sin(x))
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t *A = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!A) return 0; /* out‑of‑memory, should not happen */

    A[0] = 1;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t binom = 1; /* binom(m-1,0) */

        for (idx_t j = 0; j < m; ++j) {
            /* k = j+1 */
            if ((j & 1) == 0) {          /* k odd  -> j even   */
                idx_t t = (j) / 2;      /* (k-1)/2 = j/2       */
                seq_t term = binom * A[m - (j + 1)];
                if (t & 1)              /* sign = (-1)^(t) */
                    sum -= term;
                else
                    sum += term;
            }

            /* advance binomial coefficient to j+1 */
            if (j + 1 < m) {
                binom = binom * (seq_t)(m - 1 - j);
                binom = binom / (seq_t)(j + 1);   /* exact division */
            }
        }

        A[m] = (seq_t)n * sum;
    }

    seq_t result = A[n];
    free(A);
    return result;
}
