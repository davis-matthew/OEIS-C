/*
 * A319575  a(n) = (2/3)*n*(n^3 - 6*n^2 + 11*n - 3).
 * https://oeis.org/A319575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2/3)*n*(n^3-6*n^2+11*n-3)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 283, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if (N == 0) return 0;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t expr = n3 - 6 * n2 + 11 * N - 3;
    seq_t prod = N * expr;
    seq_t result = (prod / 3) * 2;
    return result;
}
