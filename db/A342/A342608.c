/*
 * A342608  a(n) = Sum_{d|n} phi(d)^(n+d).
 * https://oeis.org/A342608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} phi(d)^(n+d)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t phi_d = calc_phi((seq_t)d);
            seq_t exp_d = (seq_t)n + (seq_t)d;
            sum += big_pow(phi_d, exp_d);

            /* complementary divisor n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d) {
                seq_t phi_d2 = calc_phi((seq_t)d2);
                seq_t exp_d2 = (seq_t)n + (seq_t)d2;
                sum += big_pow(phi_d2, exp_d2);
            }
        }
    }
    return sum;
}
