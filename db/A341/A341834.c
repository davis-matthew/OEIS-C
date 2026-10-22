/*
 * A341834  Dirichlet g.f.: 1 / zeta(s)^8.
 * https://oeis.org/A341834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   multiplicative function with a(p^e) = (-1)^e * C(8, e)
 * Range:     i8 n <= 11, i16 n <= 1259, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t binom8[9] = {1, 8, 28, 56, 70, 56, 28, 8, 1};

seq_t a(idx_t n) {
    if (n == 1) return 1;
    idx_t temp = n;
    seq_t result = 1;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e > 8) return 0;
            seq_t term = binom8[e];
            if (e & 1) term = -term;
            result *= term;
        }
    }
    if (temp > 1) {               /* remaining prime factor with exponent 1 */
        result *= -8;
    }
    return result;
}
