/*
 * A242853  a(n) = 256*n^8 - 448*n^6 + 240*n^4 - 40*n^2 + 1.
 * https://oeis.org/A242853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 256*n^8 - 448*n^6 + 240*n^4 - 40*n^2 + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 128, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t N  = (seq_t)n;
    seq_t f1 = (seq_t)(2 * N - 1);
    seq_t f2 = (seq_t)(2 * N + 1);

    seq_t n2 = N * N;
    seq_t n3 = n2 * N;

    seq_t g1 = (seq_t)(8 * n3 - 6 * N - 1);
    seq_t g2 = (seq_t)(8 * n3 - 6 * N + 1);

    seq_t prod_f = f1 * f2;   /* (2n-1)*(2n+1) */
    seq_t prod_g = g1 * g2;   /* (8n^3-6n-1)*(8n^3-6n+1) */

    return prod_f * prod_g;   /* final product */
}
