/*
 * A272298  a(n) = n^4 + 324.
 * https://oeis.org/A272298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 324
 * Range:     u8 none, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t y = x * x;        /* x² */
    seq_t z = y * y;        /* x⁴ */
    return z + (seq_t)324;
}
