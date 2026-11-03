/*
 * A282356  Eisenstein series E_26(q) (alternate convention E_13(q)),
 *          multiplied by 657931.
 * https://oeis.org/A282356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -24 * sigma_{25}(n) for n >= 1, a(0) = 657931 where
 *            sigma_{k}(n) = sum_{d|n} d^k
 * Range:     i8 none, i16 none, i32 n <= 2, i64 n <= 5, i128 n <= 29,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 938 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow25(seq_t base) {
    seq_t res = 1;
    for (int i = 0; i < 25; ++i)
        res *= base;
    return res;
}

static seq_t sigma25(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;
    for (idx_t p = 2; (seq_t)p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t factor_power = pow25((seq_t)p);
            seq_t term = 1;       /* sum_{k=0}^{exp} factor_power^k */
            seq_t cur   = 1;       /* factor_power^(k-1) */
            for (idx_t k = 1; k <= exp; ++k) {
                cur *= factor_power;
                term += cur;
            }
            result *= term;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        seq_t factor_power = pow25((seq_t)temp);
        seq_t term = 1 + factor_power;   /* exponent is 1 */
        result *= term;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)657931;
    seq_t sigma = sigma25(n);
    return - (seq_t)24 * sigma;
}
