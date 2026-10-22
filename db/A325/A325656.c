/*
 * A325656  a(n) = (1/24)*n*((4*n + 3)*(2*n^2 + 1) - 3*(-1)^n).
 * https://oeis.org/A325656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)*n*((4*n + 3)*(2*n^2 + 1) - 3*(-1)^n)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 335, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 1) == 0) {                 /* even n */
        seq_t n2   = N * N;
        seq_t inner = (seq_t)4 * n2 + (seq_t)3 * N + (seq_t)2;
        seq_t num   = n2 * inner;
        return num / (seq_t)12;
    } else {                            /* odd n */
        seq_t n2   = N * N;
        seq_t inner = (seq_t)4 * n2 - N + (seq_t)3;
        seq_t num   = N * (N + (seq_t)1) * inner;
        return num / (seq_t)12;
    }
}
