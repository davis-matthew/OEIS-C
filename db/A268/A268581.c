/*
 * A268581  a(n) = 2*n^2 + 8*n + 5.
 * https://oeis.org/A268581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 8*n + 5
 * Range:     u8 n <= 9, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t res = (seq_t)2 * m * m + (seq_t)8 * m + (seq_t)5;
    return res;
}
