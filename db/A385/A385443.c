/*
 * A385443  Expansion of e.g.f. (1/x) * Series_Reversion( x/(3*x +
 *          sqrt(9*x^2+1))^(1/3) ).
 * https://oeis.org/A385443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∏_{k=1}^{n-1} (4*n+1-6*k), a(0) = 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t result = (seq_t)1;
    seq_t first = (seq_t)4 * (seq_t)n + (seq_t)1;
    for (idx_t k = (idx_t)1; k < n; ++k) {
        seq_t term = first - (seq_t)6 * (seq_t)k;
        result *= term;
    }
    return result;
}
