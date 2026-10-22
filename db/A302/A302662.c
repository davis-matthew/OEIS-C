/*
 * A302662  Imaginary part of Product_{k=1..n} (k+(n-k+1)i), with i=sqrt(-1).
 * https://oeis.org/A302662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Im(∏_{k=1..n} (k + (n-k+1)i))
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t r = 1;          /* real part of the running product */
    seq_t im = 0;         /* imaginary part of the running product */
    seq_t N = (seq_t)n + 1;   /* constant n+1 used for b_k */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t a_k = (seq_t)k;
        seq_t b_k = N - a_k;   /* n+1-k */

        seq_t new_r = r * a_k - im * b_k;
        seq_t new_im = r * b_k + im * a_k;

        r = new_r;
        im = new_im;
    }

    return im;
}
