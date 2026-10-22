/*
 * A383466  a(0) = 1; thereafter a(n) = 10*n^2 - 5*n + 2.
 * https://oeis.org/A383466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 10*n^2 - 5*n + 2 for n > 0
 * Range:     u8 n <= 5, u16 n <= 81, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t N = (seq_t)n;
    return (seq_t)10 * N * N - (seq_t)5 * N + (seq_t)2;
}
