/*
 * A347536  Number of minimum dominating sets in the complete tripartite graph
 *          K_{n,n,n}.
 * https://oeis.org/A347536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*n, except a(2) = 15
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) is 3, a(2) is 15, a(n) = 3*n^2 for n != 2 */

seq_t a(idx_t n)
{
    /* special case for n = 2 */
    if (n == 2)
        return (seq_t)15;

    /* compute 3 * n * n using seq_t arithmetic */
    seq_t N = (seq_t)n;
    return (seq_t)3 * N * N;
}
