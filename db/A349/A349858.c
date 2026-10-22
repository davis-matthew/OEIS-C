/*
 * A349858  Expansion of Sum_{k>=0} x^k/(1 + k^4 * x).
 * https://oeis.org/A349858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-k^4)^(n-k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 11, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq_small(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t base = -(kk * kk * kk * kk);
        idx_t exp = n - k;
        seq_t term = pow_seq_small(base, exp);
        sum += term;
    }
    return sum;
}
