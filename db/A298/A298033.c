/*
 * A298033  Coordination sequence of the Dual(3.4.6.4) tiling with respect to
 *          a hexavalent node.
 * https://oeis.org/A298033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0 a(n)=6*floor((3*n-1)/2) which equals 9*n-6 if n
 *            even and 9*n-3 if n odd
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    return 6 * ((3 * nn - 1) / 2);
}
