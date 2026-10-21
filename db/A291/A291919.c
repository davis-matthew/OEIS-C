/*
 * A291919  Number of (undirected) paths in the n-wheel graph.
 * https://oeis.org/A291919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)*(2*n^3 - 12*n^2 + 43*n - 48)/6
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t once to avoid repeated casts */
    seq_t N   = (seq_t)n;
    seq_t Nm1 = N - (seq_t)1;

    /* Compute n^2 and n^3 */
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;

    /* Compute the polynomial: 2*n^3 - 12*n^2 + 43*n - 48 */
    seq_t term = 2 * n3;      /* 2*n^3 */
    term -= 12 * n2;          /* -12*n^2 */
    term += 43 * N;           /* +43*n */
    term -= 48;               /* -48 */

    /* Final multiplication and division */
    return (Nm1 * term) / (seq_t)6;
}
