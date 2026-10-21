/*
 * A292161  Main diagonal of A292159.
 * https://oeis.org/A292161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the real part of the coefficient of x^n in *
 *            Product_{k>=0} (1 - n*i*x^k), where i^2 = -1. * Implementation:
 *            dynamic programming over subsets of distinct positive integers.
 *            * For each n, let real[s], imag[s] be the real and imaginary
 *            part of the * coefficient of x^s in ∏_{k=1}^{n} (1 - n*...
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 22, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle decimal 0 directly   */
    if (n == 0) return (seq_t)1;

    /* arrays to hold real and imaginary parts up to index n */
    seq_t *real = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *imag = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (real == NULL || imag == NULL) return (seq_t)0;   /* allocation failed */

    for (idx_t i = 0; i <= n; ++i) {
        real[i] = (seq_t)0;
        imag[i] = (seq_t)0;
    }
    real[0] = (seq_t)1;   /* initial term: coefficient of x^0 is 1 */

    seq_t mult = (seq_t)n;   /* common factor -n*i will be applied as -n and i */

    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t s = n; s >= k; --s) {
            seq_t inc_real = imag[s - k] * mult;          /* n * imag[s-k]   */
            seq_t inc_imag = -(real[s - k] * mult);      /* -n * real[s-k]  */

            real[s] = real[s] + inc_real;
            imag[s] = imag[s] + inc_imag;
        }
    }

    seq_t result = real[n] + mult * imag[n];

    free(real);
    free(imag);

    return result;
}
