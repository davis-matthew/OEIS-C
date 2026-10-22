/*
 * A370192  a(n) is the numerator of the imaginary part of 1/(1+i/2)^n, where
 *          i is the imaginary unit.
 * https://oeis.org/A370192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = 8*a(n-1) - 20*a(n-2), a(0) = 0, a(1) = -2
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 29, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return -2;

    seq_t a_nm2 = 0;      /* a(n-2) */
    seq_t a_nm1 = -2;     /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i)
    {
        a_n = 8 * a_nm1 - 20 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
