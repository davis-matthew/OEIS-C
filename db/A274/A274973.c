/*
 * A274973  Centered cubohemioctahedral numbers: a(n) = 2*n^3+9*n^2+n+1.
 * https://oeis.org/A274973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3+9*n^2+n+1
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    return 2 * N3 + 9 * N2 + N + (seq_t)1;
}
