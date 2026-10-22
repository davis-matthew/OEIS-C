/*
 * A366478  a(n) = (3*n^4 - 4*n^3 + n^2 + 4*n + 4)/4.
 * https://oeis.org/A366478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^4 - 4*n^3 + n^2 + 4*n + 4)/4
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m  = (seq_t)n;
    seq_t n2 = m * m;
    seq_t n3 = n2 * m;
    seq_t n4 = n3 * m;
    seq_t term = 3 * n4 - 4 * n3 + n2 + 4 * m + 4;
    return term / 4;
}
