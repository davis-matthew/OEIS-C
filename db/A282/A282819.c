/*
 * A282819  Number of inequivalent ways to color the edges of a tetrahedron
 *          using at most n colors so that no two opposite edges have the same
 *          color.
 * https://oeis.org/A282819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^4-2*n^3+n^2+8)/12
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the expression is 0. Avoid unsigned underflow. */
    if (n < 2)
        return 0;

    seq_t nn = (seq_t)n;           /* n */
    seq_t n1 = nn - 1;             /* n-1 */

    /* Compute powers of n */
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */
    seq_t n4 = n3 * nn;            /* n^4 */

    /* Compute the polynomial term: n^4 - 2*n^3 + n^2 + 8 */
    seq_t term = n4 - (seq_t)2 * n3 + n2 + (seq_t)8;

    /* Compute the numerator: n*(n-1)*term */
    seq_t numerator = (nn * n1) * term;

    /* Divide exactly by 12 */
    return numerator / (seq_t)12;
}
