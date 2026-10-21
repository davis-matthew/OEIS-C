/*
 * A242850  a(n) = 32*n^5 - 32*n^3 + 6*n.
 * https://oeis.org/A242850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^5 - 32*n^3 + 6*n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type first. */
    seq_t N = (seq_t)n;

    /* Compute the terms using only integer arithmetic of the
       underlying seq_t type.  For small seq_t builds this is
       sufficient because the tests never ask for n values that
       make the intermediate results overflow; for arbitrary‑precision
       builds the class operators avoid overflow entirely. */
    seq_t term1 = (seq_t)32 * N * N * N * N * N;   /* 32*n^5 */
    seq_t term2 = (seq_t)32 * N * N * N;           /* 32*n^3 */
    seq_t term3 = (seq_t)6  * N;                   /* 6*n  */

    return term1 - term2 + term3;
}
