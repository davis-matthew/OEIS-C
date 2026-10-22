/*
 * A387684  a(n) = (8*n + 1)^2*(4*n + 1).
 * https://oeis.org/A387684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 1)^2*(4*n + 1)
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t t = 8 * sn + 1;
    t = t * t;
    seq_t s = 4 * sn + 1;
    return t * s;
}
