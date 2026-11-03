/*
 * A363610  Expansion of Sum_{k>0} x^(3*k)/(1-x^k)^3.
 * https://oeis.org/A363610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} binomial(d-1,2)
 * Range:     u8 n <= 23, u16 n <= 299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9061 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom2minus(idx_t d) {
    if (d < 3) return 0;
    idx_t a = d - 1;
    idx_t b = d - 2;
    /* divide by 2 early to avoid overflow */
    if ((a & 1) == 0) {
        a /= 2;
    } else {
        b /= 2;
    }
    return (seq_t)a * (seq_t)b;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += binom2minus(i);
            idx_t j = n / i;
            if (j != i) {
                sum += binom2minus(j);
            }
        }
    }
    return sum;
}
