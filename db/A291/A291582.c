/*
 * A291582  Maximum number of 6 sphinx tile shapes in a sphinx tiled hexagon
 *          of order n.
 * https://oeis.org/A291582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n*(6*n-1)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t x = (seq_t)6 * nn;          /* x = 6*n */
    return x * (x - 1);               /* a(n) = x*(x-1) = 6*n*(6*n-1) */
}
