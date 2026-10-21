/*
 * A266087  Alternating sum of 11-gonal (or hendecagonal) numbers.
 * https://oeis.org/A266087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((18*n^2 + 4*n - 7)*(-1)^n + 7)/8
 * Range:     i8 n <= 2, i16 n <= 42, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t sign = (n & 1) ? (seq_t)(-1) : (seq_t)1;
    seq_t num = (18 * N * N + 4 * N - 7) * sign + 7;
    return num / 8;
}
