/*
 * A359980  Expansion of Product_{k>=0} (1 - x^(k^2+2)) in powers of x.
 * https://oeis.org/A359980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = coefficient of x^n in ∏_{k>=0} (1 - x^{k^2+2})
 * Range:     i8 n <= 730, i16 n <= 3153, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple dynamic programming recurrence
   that is equivalent to multiplying out the product term by term. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate workspace for coefficients up to n */
    size_t sz = (size_t)(n + 1);
    seq_t *coeff = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!coeff)
        return (seq_t)0;          /* allocation failure → zero (unlikely) */

    /* Initial state: coeff[0] = 1, others 0 */
    coeff[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        coeff[i] = (seq_t)0;

    /* Iteratively apply each factor (1 - x^m) with m = k^2 + 2 */
    for (idx_t k = 0; ; ++k) {
        idx_t m = k * k + 2;
        if (m > n)
            break;
        for (idx_t j = n; j >= m; --j) {
            coeff[j] = coeff[j] - coeff[j - m];
        }
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
