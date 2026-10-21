/*
 * A292681  Rule 6: (000, ..., 111) -> (0, 1, 1, 0, 0, 0, 0, 0), without
 *          extending to the right of input bit 0.
 * https://oeis.org/A292681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (~(n >> 1)) & (n ^ (n << 1))
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    return (~(m >> 1)) & (m ^ (m << 1));
}
