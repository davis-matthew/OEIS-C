/*
 * A326814  Dirichlet g.f.: (1/zeta(s)) * Product_{p prime} (1 - 2 * p^(-s)).
 * https://oeis.org/A326814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = ∏_{p^e∥n} f(p^e) where f(p^e) = -3 if e=1, 2 if e=2, and
 *            0 otherwise
 * Range:     i8 n <= 2309, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t term;
            if (exp == 1) {
                term = -3;
            } else if (exp == 2) {
                term = 2;
            } else {
                term = 0;
            }
            result *= term;
            if (result == 0) return 0;
        }
    }

    if (m > 1) {          /* remaining prime factor with exponent 1 */
        result *= -3;
    }

    return result;
}
