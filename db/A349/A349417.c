/*
 * A349417  a(n) is the Wiener index of a sling on n+1 vertices.
 * https://oeis.org/A349417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (n^3 + 3*n^2 - 4*n + 12)/6
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 2268, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = n3 + 3 * n2 - 4 * nn + (seq_t)12;
    return num / 6;
}
