/*
 * A282099  Coefficients in q-expansion of (E_2^2*E_4 - 2*E_2*E_6 +
 *          E_4^2)/1728, where E_2, E_4, E_6 are the Eisenstein series shown
 *          in A006352, A004009, A013973, respectively.
 * https://oeis.org/A282099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^2 * sigma_3(n), where sigma_3(m) is the sum of cubes of
 *            divisors of m
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 82, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t m = n;
    seq_t sigma = 1;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t pseq = (seq_t)p;

            /* numerator = p^{3*(e+1)} - 1 */
            seq_t num = 1;
            for (idx_t i = 0; i < 3 * (e + 1); ++i) num *= pseq;
            num -= 1;

            /* denominator = p^3 - 1 */
            seq_t den = 1;
            for (idx_t i = 0; i < 3; ++i) den *= pseq;
            den -= 1;

            sigma *= num / den;
        }
    }

    if (m > 1) { /* remaining prime factor with exponent 1 */
        seq_t pseq = (seq_t)m;

        seq_t num = 1;
        for (idx_t i = 0; i < 6; ++i) num *= pseq; /* 3*(1+1)=6 */
        num -= 1;

        seq_t den = 1;
        for (idx_t i = 0; i < 3; ++i) den *= pseq;
        den -= 1;

        sigma *= num / den;
    }

    seq_t res = (seq_t)n;
    res *= res;
    res *= sigma;
    return res;
}
