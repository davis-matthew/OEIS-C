/*
 * A374397  a(n) is the number of 4-step self avoiding walks in the
 *          n-dimensional hypercubic lattice that start at the origin.
 * https://oeis.org/A374397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 16*n^4 - 24*n^3 + 8*n^2 + 2*n (OEIS A374397)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 128, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t first to avoid overflow in intermediate steps. */
    seq_t nn = (seq_t)n;

    /* Compute powers of n: n^2, n^3, n^4 */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n2 * n2;           /* n^4 */

    /* Evaluate the polynomial using only seq_t arithmetic. */
    seq_t result = 16 * n4;       /* 16*n^4 */
    result -= 24 * n3;            /* -24*n^3 */
    result += 8 * n2;             /* +8*n^2 */
    result += 2 * nn;             /* +2*n */

    return result;
}
