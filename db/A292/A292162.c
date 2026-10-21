/*
 * A292162  Main diagonal of A292160.
 * https://oeis.org/A292162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Im((1 - n*i) * [x^n] ∏_{k=1}^{∞} (1 - n*i*x^k))
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 21, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A292162 (main diagonal of A292160). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Allocate arrays for the real and imaginary parts of the coefficients. */
    seq_t *real = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *imag = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!real || !imag) {
        /* In case of allocation failure, return zero (not expected). */
        free(real);
        free(imag);
        return 0;
    }

    /* Initialize to zero. */
    for (idx_t i = 0; i <= n; ++i) {
        real[i] = 0;
        imag[i] = 0;
    }
    real[0] = 1;
    imag[0] = 0;

    /* Main DP: process part sizes sequentially. */
    for (idx_t k = 1; k <= n; ++k) {
        /* Descending loop to avoid using an added part more than once. */
        for (idx_t s = n; ; --s) {
            /* Compute contribution when part k is taken. */
            seq_t add_real = imag[s - k] * (seq_t)n;   /* n * imag[s-k]   */
            seq_t add_imag = -(seq_t)n * real[s - k];  /* -n * real[s-k]  */

            real[s] += add_real;
            imag[s] += add_imag;

            if (s == k)  /* finished the loop for this k */
                break;
        }
    }

    /* Compute the final value: Im((1 - n*i)*dp[n]) = imag[n] - n*real[n]. */
    seq_t result = imag[n] - (seq_t)n * real[n];

    free(real);
    free(imag);
    return result;
}
