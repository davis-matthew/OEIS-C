/*
 * A334563  a(n) is the maximum number of 4-cycles possible in an n-vertex
 *          planar graph.
 * https://oeis.org/A334563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n ≤ 3; a(n) = (n^2 + 3*n - 22)/2 for n ≥ 4
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn + 3 * nn - 22;
    return val / 2;
}
