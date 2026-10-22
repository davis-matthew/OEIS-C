/*
 * A392174  One-fourth of the total number of edges in the graph (see A392172)
 *          formed when n points are placed in general position on each edge
 *          of a square and a chord is drawn from each point to the 3*n points
 *          on the other three sides.
 * https://oeis.org/A392174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n + (9/4)*n^2 - (9/2)*n^3 + (17/4)*n^4
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 177, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* n */
    seq_t result = 1 + nn;        /* 1 + n */
    result += (seq_t)9 * nn * nn / 4;              /* + (9/4)*n^2 */
    result -= (seq_t)9 * nn * nn * nn / 2;         /* - (9/2)*n^3 */
    result += (seq_t)17 * nn * nn * nn * nn / 4;   /* + (17/4)*n^4 */
    return result;
}
