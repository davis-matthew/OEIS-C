/*
 * A298038  Coordination sequence of Dual(4.6.12) tiling with respect to a
 *          hexavalent node.
 * https://oeis.org/A298038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=12*n if n is even, otherwise 6*n
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n & 1) return (seq_t)n * (seq_t)6;
    return (seq_t)n * (seq_t)12;
}
