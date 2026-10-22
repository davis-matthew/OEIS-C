/*
 * A342620  a(n) = Sum_{d|n} phi(n/d)^(n+d+1).
 * https://oeis.org/A342620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d)^(n + d + 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* first divisor */
            idx_t d1 = i;
            idx_t m1 = n / d1;
            seq_t phi_m1 = phi(m1);
            seq_t exp1 = (seq_t)n + (seq_t)d1 + 1;
            seq_t pow1 = big_pow(phi_m1, exp1);
            result += pow1;
            /* second divisor (if distinct) */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t m2 = n / d2;
                seq_t phi_m2 = phi(m2);
                seq_t exp2 = (seq_t)n + (seq_t)d2 + 1;
                seq_t pow2 = big_pow(phi_m2, exp2);
                result += pow2;
            }
        }
    }
    return result;
}
