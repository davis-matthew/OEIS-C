/*
 * A244728  a(n) = 9*n^3.
 * https://oeis.org/A244728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^3
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 781, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t n2 = n0 * n0;
    seq_t n3 = n2 * n0;
    return 9 * n3;
}
