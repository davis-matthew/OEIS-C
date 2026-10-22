/*
 * A387286  Number of 2 X 2 square tiles in a discrete 4-dimensional hypercube
 *          of side length n.
 * https://oeis.org/A387286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^2 * (3*n^2 + 2*n) = 3*n^4 - 4*n^3 - n^2 + 2*n
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the output type to avoid mixing signed/unsigned and
       to guarantee that all arithmetic is performed in seq_t */
    seq_t nn = (seq_t)n;

    /* compute (n-1)^2 */
    seq_t nfrac = nn - (seq_t)1;
    seq_t part1 = nfrac * nfrac;           /* (n-1)^2 */

    /* compute 3*n^2 + 2*n */
    seq_t n2    = nn * nn;                 /* n^2 */
    seq_t part2 = (seq_t)3 * n2 + (seq_t)2 * nn;

    /* final product */
    return part1 * part2;
}
