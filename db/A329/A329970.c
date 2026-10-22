/*
 * A329970  a(n) = (-1)^(n + 1) * n * ceiling(n/2) + Sum_{k=1..n} (-1)^k * k^2
 *          * floor(n/k).
 * https://oeis.org/A329970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * (n mod k) * k
 * Range:     i8 n <= 23, i16 n <= 357, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t sign = (k & 1) ? (seq_t)1 : (seq_t)-1;
        seq_t mod = (seq_t)(n % k);
        seq_t term = sign * mod * (seq_t)k;
        result += term;
    }
    return result;
}
