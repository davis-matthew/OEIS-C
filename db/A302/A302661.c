/*
 * A302661  Real part of Product_{k=1..n} (k+(n-k+1)i), with i=sqrt(-1).
 * https://oeis.org/A302661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Re(∏_{k=1..n} (k + (n-k+1)i))
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Re(1) and Im(1) of the empty product */
    seq_t real = 1;
    seq_t imag = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* k  and  (n-k+1) are the real and imaginary parts of the k-th factor */
        seq_t k_real   = (seq_t)k;
        seq_t k_imag   = (seq_t)(n - k + 1);

        seq_t new_real = real * k_real - imag * k_imag;
        seq_t new_imag = real * k_imag + imag * k_real;

        real = new_real;
        imag = new_imag;
    }

    return real;  /* return the real part of the product */
}
