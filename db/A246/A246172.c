/*
 * A246172  a(n) = (n^2 + 9*n - 8)/2.
 * https://oeis.org/A246172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 9*n - 8)/2
 * Range:     u8 n <= 18, u16 n <= 357, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t first to avoid overflow in intermediate products */
    seq_t nn = (seq_t)n;
    return (nn * (nn + 9) - 8) / 2;
}
