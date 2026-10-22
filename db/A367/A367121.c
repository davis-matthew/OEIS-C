/*
 * A367121  Place n points in general position on each side of a square, and
 *          join every pair of the 4*n+4 boundary points by a chord; sequence
 *          gives number of regions in the resulting planar graph.
 * https://oeis.org/A367121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (17/2)*n^4 + 19*n^3 + (43/2)*n^2 + 14*n + 4
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute the fractional part together to avoid truncation errors. */
    seq_t half_n4 = (seq_t)17 * nn * nn * nn * nn;  /* 17*n^4   */
    seq_t half_n2 = (seq_t)43 * nn * nn;            /* 43*n^2   */
    seq_t term_half = (half_n4 + half_n2) / 2;     /* (17*n^4+43*n^2)/2 */

    seq_t term3 = (seq_t)19 * nn * nn * nn;         /* 19*n^3   */
    seq_t term4 = (seq_t)14 * nn;                   /* 14*n     */

    return term_half + term3 + term4 + 4;
}
