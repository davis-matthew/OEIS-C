/*
 * A302298  Wiener index of the graph of nodes (i,j) of the square lattice
 *          such that abs(i) + abs(j) <= n.
 * https://oeis.org/A302298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(6 + 25*n + 40*n^2 + 35*n^3 + 14*n^4) / 15
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;        /* n^2 */
    seq_t n3 = n2 * N;       /* n^3 */
    seq_t n4 = n3 * N;       /* n^4 */
    seq_t expr = (seq_t)6;   /* constant term */
    expr += (seq_t)25 * N;   /* 25*n */
    expr += (seq_t)40 * n2;  /* 40*n^2 */
    expr += (seq_t)35 * n3;  /* 35*n^3 */
    expr += (seq_t)14 * n4;  /* 14*n^4 */
    /* Multiply by 2*N and divide by 15 (division is exact) */
    seq_t res = ((seq_t)2 * N * expr) / (seq_t)15;
    return res;
}
