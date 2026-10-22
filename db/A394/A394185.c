/*
 * A394185  a(n) = n^3/6 + 3*n^2 + 41*n/6 + 3.
 * https://oeis.org/A394185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 18*n^2 + 41*n)/6 + 3
 * Range:     u8 n <= 2, u16 n <= 34, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t un = n;
    seq_t n2 = un * un;
    seq_t n3 = n2 * un;
    seq_t numerator = n3 + 18 * n2 + 41 * un;
    return numerator / 6 + 3;
}
