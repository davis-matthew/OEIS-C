/*
 * A298027  Partial sums of A298026.
 * https://oeis.org/A298027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4 + 6*n + 9*n^2)/4 if n is even * a(n) = (7 +12*n +
 *            9*n^2)/4 if n is odd
 * Range:     u8 n <= 4, u16 n <= 169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 1u) == 0) {
        seq_t numer = (seq_t)4 + (seq_t)6 * N + (seq_t)9 * N * N;
        return numer / (seq_t)4;
    } else {
        seq_t numer = (seq_t)7 + (seq_t)12 * N + (seq_t)9 * N * N;
        return numer / (seq_t)4;
    }
}
