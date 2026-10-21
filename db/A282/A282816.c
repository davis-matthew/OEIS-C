/*
 * A282816  Number of inequivalent ways to color the faces of a cube using at
 *          most n colors so that no two opposite sides have the same color.
 * https://oeis.org/A282816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^4-2*n^3+n^2+8)/24
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the lower offsets explicitly to avoid unsigned underflow. */
    if (n < 2)
        return 0;

    seq_t nn    = (seq_t)n;
    seq_t n2    = nn * nn;
    seq_t n3    = n2 * nn;
    seq_t n4    = n3 * nn;
    seq_t inner = n4 - (seq_t)2 * n3 + n2 + (seq_t)8;
    seq_t numerator = nn * (nn - (seq_t)1) * inner;
    return numerator / (seq_t)24;
}
