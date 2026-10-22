/*
 * A399228  a(n) = Sum_{k=0..n} (-1)^k*(n - k)!*(n + k)!.
 * https://oeis.org/A399228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((n!)^2 + (-1)^n*(2*n + 1)!/(n + 1))/2
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact = fact * (seq_t)i;

    seq_t fact_sq = fact * fact;

    /* C(2n+1, n) with multiplicative formula */
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i) {
        binom = (binom * (seq_t)(n + 1 + i)) / (seq_t)i;
    }

    seq_t sign_binom = (n & 1) ? -binom : binom;
    seq_t numerator = fact_sq * (seq_t)(1 + sign_binom);
    return numerator / 2;
}
