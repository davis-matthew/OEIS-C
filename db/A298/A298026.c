/*
 * A298026  Coordination sequence of Dual(3.6.3.6) tiling with respect to a
 *          hexavalent node.
 * https://oeis.org/A298026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(2k)=6*k, a(2k+1)=12*k+6
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if ((n & 1ULL) == 0)        /* even */
        return (seq_t)n * (seq_t)3;
    else                        /* odd */
        return (seq_t)n * (seq_t)6;
}
