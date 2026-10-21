/*
 * A268810  a(n) = 2*floor(3*n*(n+1)/4).
 * https://oeis.org/A268810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(3*n*(n+1)/4)
 * Range:     u8 n <= 8, u16 n <= 207, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute 3*n*(n+1) */
    seq_t t = (seq_t)3 * nn * (nn + 1);
    /* Modulo 4 of t using bitwise AND (since 4 is a power of 2) */
    seq_t r = t & 3;
    /* a(n) = (t - r) / 2 */
    return (t - r) / 2;
}
