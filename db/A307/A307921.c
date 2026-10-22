/*
 * A307921  Number of (undirected) paths in the n-book graph.
 * https://oeis.org/A307921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 + n^2 + 6*n + 1
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first */
    seq_t nn = (seq_t)n;

    /* Compute each term of the polynomial */
    seq_t term1 = 4 * nn * nn * nn; /* 4 * n^3 */
    seq_t term2 = nn * nn;          /* n^2 */
    seq_t term3 = 6 * nn;           /* 6 * n */

    /* Sum the terms and add the constant 1 */
    return term1 + term2 + term3 + 1;
}
