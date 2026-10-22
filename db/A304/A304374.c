/*
 * A304374  a(n) = 9*n^2 + 21*n - 6 (n>=1).
 * https://oeis.org/A304374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 + 21*n - 6
 * Range:     u8 n <= 4, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 9 * nn * nn + 21 * nn - 6;
}
