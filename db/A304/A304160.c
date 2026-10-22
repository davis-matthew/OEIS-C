/*
 * A304160  a(n) = n^4 - 3*n^3 + 6*n^2 - 5*n + 2 (n >= 1).
 * https://oeis.org/A304160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 3*n^3 + 6*n^2 - 5*n + 2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long N = n;
    long long val = N*N*N*N - 3*N*N*N + 6*N*N - 5*N + 2;
    return (seq_t)val;
}
