/*
 * A367117  Place n points in general position on each side of an equilateral
 *          triangle, and join every pair of the 3*n+3 boundary points by a
 *          chord; sequence gives number of vertices in the resulting planar
 *          graph.
 * https://oeis.org/A367117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/4) * (n+1) * (3*n^3 + n^2 + 4)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 207, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n1 = nn + 1;

    seq_t t1 = 3 * nn * nn * nn;   /* 3*n^3 */
    seq_t t2 = nn * nn;            /* n^2 */

    seq_t sum = t1 + t2 + 4;       /* 3*n^3 + n^2 + 4 */
    seq_t prod = n1 * sum * 3;     /* (n+1)*(3*n^3 + n^2 + 4)*3 */

    return prod / 4;               /* divide by 4 as per formula */
}
