/*
 * A328005  Number of distinct coefficients in functional composition of 1 + x
 *          + ... + x^(n-1) with itself.
 * https://oeis.org/A328005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + (-1)^n + 3)/4 for n >= 5; a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=8
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 4;
    if (n == 4) return 8;
    seq_t m = (seq_t)n;
    seq_t val = 2 * m * m + ((m & 1u) ? 2 : 4);
    return val / 4;
}
