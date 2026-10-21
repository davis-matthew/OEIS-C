/*
 * A267313  Expansion of x*(-1 + 2*x + 3*x^2 - 2*x^3 + x^4)/((1 - x)^3*(1 + x
 *          + x^2)^2).
 * https://oeis.org/A267313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k = 0..n} (-1)^(k mod 3) * k
 * Range:     i8 n <= 26, i16 n <= 440, i32 n <= 9636, i64 n <= 9636,
 *            i128 n <= 9636, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the definition directly. */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = (k % 3 == 1) ? -(seq_t)k : (seq_t)k;
        sum += term;
    }
    return sum;
}
