/*
 * A350716  a(n) is the minimum number of vertices of degree 3 over all
 *          3-collapsible graphs with n vertices.
 * https://oeis.org/A350716
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 for 4 ≤ n ≤ 7, otherwise ceil(2*n/5)
 * Range:     u8 n <= 279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (nn < 4)        /* below offset – value irrelevant */
        return 0;
    if (nn <= 7)       /* a(4) … a(7) */
        return 4;
    return (2 * nn + 4) / 5;   /* ceil(2*n/5) = (2*n + 4)/5 */
}
