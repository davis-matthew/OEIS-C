/*
 * A299269  Partial sums of A299268.
 * https://oeis.org/A299269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (20*n^3 + 33*n^2 - 2*n + 12)/12 for n even, * a(n) =
 *            (20*n^3 + 27*n^2 + 28*n + 9)/12 for n odd
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 654, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 1) == 0) {
        return (20 * N * N * N + 33 * N * N - 2 * N + 12) / 12;
    } else {
        return (20 * N * N * N + 27 * N * N + 28 * N + 9) / 12;
    }
}
