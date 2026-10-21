/*
 * A270510  Number of 2X2X2 triangular 0..n arrays with some element plus some
 *          adjacent element totalling n+1 exactly once.
 * https://oeis.org/A270510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n^2 - 3*n, if n is even; * a(n) = 3*n^2 - 3*n + 3, if n
 *            is odd
 * Range:     u8 n <= 9, u16 n <= 148, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast index to the seq_t type */
    seq_t term = 3 * N * N - 3 * N; /* 3*n^2 - 3*n */
    if (n & 1)                    /* odd n */
        term += 3;
    return term;
}
