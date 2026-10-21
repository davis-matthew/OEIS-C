/*
 * A294172  Maximum value of the cyclic convolution of the first n positive
 *          integers with themselves.
 * https://oeis.org/A294172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(13 + 3*(-1)^n + 24*n + 14*n^2)/48, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not defined for offset 1, safe fallback */
    if (n == 1) return (seq_t)1;
    seq_t ns   = (seq_t)n;
    seq_t c    = (n % 2 == 0) ? (seq_t)16 : (seq_t)10;  /* 13 + 3*(-1)^n */
    seq_t num  = ns * ( (seq_t)14 * ns * ns + (seq_t)24 * ns + c );
    return num / (seq_t)48;
}
