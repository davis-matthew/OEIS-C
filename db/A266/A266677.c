/*
 * A266677  Alternating sum of hexagonal pyramidal numbers.
 * https://oeis.org/A266677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((-1)^n*(2*n*(n + 2)*(4*n + 1) - 3) + 3)/24
 * Range:     i8 n <= 1, i16 n <= 15, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t inner = ((seq_t)2) * N * (N + (seq_t)2) *
                  ((((seq_t)4) * N) + (seq_t)1) - (seq_t)3;
    if (n & (idx_t)1) {
        inner = -inner;
    }
    inner = inner + (seq_t)3;
    return inner / (seq_t)24;
}
