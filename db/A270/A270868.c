/*
 * A270868  a(n) = n^4 + 3*n^3 + 8*n^2 + 9*n + 2.
 * https://oeis.org/A270868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 3*n^3 + 8*n^2 + 9*n + 2
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
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
    seq_t n4 = n3 * n0;
    return n4 + (seq_t)3 * n3 + (seq_t)8 * n2 + (seq_t)9 * n0 + 2;
}
