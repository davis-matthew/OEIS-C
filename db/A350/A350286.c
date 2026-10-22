/*
 * A350286  Number of different ways to partition the set of vertices of a
 *          convex (n+11)-gon into 4 nonintersecting polygons.
 * https://oeis.org/A350286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n*(n+1)*(n+2)*(n+9)*(n+10)*(n+11))/144
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t prod = 1;
    prod *= sn;                /* n */
    prod *= sn + 1;            /* n+1 */
    prod *= sn + 2;            /* n+2 */
    prod *= sn + 9;            /* n+9 */
    prod *= sn + 10;           /* n+10 */
    prod *= sn + 11;           /* n+11 */
    prod /= (seq_t)144;        /* divide by 144 */
    return prod;
}
