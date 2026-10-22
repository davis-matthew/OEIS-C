/*
 * A394489  Expansion of e.g.f. (1/x) * Series_Reversion( x/(1 + 5*x)^(2/5) ).
 * https://oeis.org/A394489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = prod_{j=0}^{n-1} (2*(n+1) - 5*j) / (n+1)
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 9, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prod = 1;
    seq_t m = (seq_t)n + 1;          /* m = n + 1 */
    for (idx_t j = 0; j < n; ++j) {
        seq_t term = 2 * m - (seq_t)5 * j;  /* integer term */
        prod = prod * term;                 /* multiply with arbitrary precision if needed */
    }
    return prod / m;          /* integer division, exact by definition */
}
