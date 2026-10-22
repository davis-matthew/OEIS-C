/*
 * A397964  Number of points with even coordinate sum in the 4-dimensional
 *          hypercube [-n,n]^4 having an odd number of lattice neighbors
 *          inside the hypercube.
 * https://oeis.org/A397964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^3 - 48*n^2 + 56*n - 40
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 512, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset starts at 1, undefined for n<1 */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;               /* n^2 */
    seq_t n3 = n2 * nn;               /* n^3 */
    return 32 * n3 - 48 * n2 + 56 * nn - 40;
}
