/*
 * A365040  E.g.f. satisfies A(x) = exp(x * (1 + x)/A(x)^3).
 * https://oeis.org/A365040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * Sum_{k=ceil(n/2)}^n (-3*k+1)^(k-1) * binomial(k,
 *            n-k)/k!
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent summation over i=0..⌊n/2⌋: a(n) = Σ_{i=0}^{⌊n/2⌋} n! * (1-3(n-i))^(n-i-1) / (i! (n-2i)!) */

static seq_t pow_big(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp > 0) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    idx_t max_i = n / 2;
    seq_t sum = 0;
    seq_t S = 1;                 /* S_0 = n!/(0!·n!) = 1 */
    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t m = n - i;                         /* m = n - i  */
        seq_t base = (seq_t)1 - (seq_t)3 * (seq_t)m;
        seq_t powv = pow_big(base, m - 1);       /* exponent m-1 ≥ 0   */
        seq_t term = S * powv;
        sum += term;

        if (i != max_i) {
            /* Update S to S_{i+1} = S_i * (n-2i)(n-2i-1)/(i+1) */
            seq_t numer = (seq_t)(n - 2 * i) * (seq_t)(n - 2 * i - 1);
            S = S * numer / (i + 1);
        }
    }
    return sum;
}
