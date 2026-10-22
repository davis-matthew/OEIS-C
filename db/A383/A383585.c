/*
 * A383585  Number of vertices of even degree in a cubic lattice n X n X n.
 * https://oeis.org/A383585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - 6*n^2 + 24*n - 32 for n >= 2, a(1)=0, a(2)=0
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    seq_t N  = (seq_t)n;      /* cast index to seq_t */
    seq_t n2 = N * N;         /* N^2   */
    seq_t n3 = n2 * N;        /* N^3   */
    seq_t res = n3 - 6 * n2 + 24 * N - 32; /* polynomial */
    return res;
}
