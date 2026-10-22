/*
 * A370191  a(n) is the numerator of the real part of 1/(1+i/2)^n, where i is
 *          the imaginary unit.
 * https://oeis.org/A370191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = Re((4-2i)^n) (the numerator of the real part of
 *            1/(1+i/2)^n, whose denominator is 5^n)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 14, i64 n <= 30, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* real and imaginary parts of (4-2i)^k */
    seq_t a = 1;  /* (4-2i)^0: real part */
    seq_t b = 0;  /* (4-2i)^0: imaginary part */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t new_a = (seq_t)4 * a + (seq_t)2 * b;
        seq_t new_b = (seq_t)4 * b - (seq_t)2 * a;
        a = new_a;
        b = new_b;
    }
    return a;
}
