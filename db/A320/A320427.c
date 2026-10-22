/*
 * A320427  a(n) = floor(3*n/2) + ceiling(n/6) + 9.
 * https://oeis.org/A320427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n/2) + ceil(n/6) + 9
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn * 3 / 2) + (nn + 5) / 6 + 9;
}
