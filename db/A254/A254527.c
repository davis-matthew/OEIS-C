/*
 * A254527  Total number of points on a sphere when both poles are on an x by
 *          x grid where x=8*n+1.
 * https://oeis.org/A254527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 - 4*n + 2
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;          /* n^2 */
    val = val * (seq_t)8;          /* 8*n^2 */
    val = val - nn * (seq_t)4;     /* - 4*n */
    val = val + (seq_t)2;          /* + 2 */
    return val;
}
