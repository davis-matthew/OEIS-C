/*
 * A303812  Generalized 28-gonal (or icosioctagonal) numbers: m*(13*m - 12)
 *          with m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A303812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(13*n + 24)/4 if n is even, * (n+1)*(13*n - 11)/4 if n
 *            is odd
 * Range:     u8 n <= 7, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 1) == 0) { /* n even */
        return N * (((seq_t)13 * N) + (seq_t)24) / (seq_t)4;
    } else {          /* n odd */
        return (N + (seq_t)1) * (((seq_t)13 * N) - (seq_t)11) / (seq_t)4;
    }
}
