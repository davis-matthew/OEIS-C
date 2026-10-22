/*
 * A342485  a(n) = Sum_{d|n} phi(d)^(d+1).
 * https://oeis.org/A342485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)^(d+1) (OEIS A342485)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"     /* provides seq_t phi(idx_t m)          */
#include "../utils/big_pow.h" /* provides seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* 0 is not used, just safe          */

    seq_t sum = 0;

    for (idx_t i = 1; (__uint128_t)i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t phi1 = phi(d1);
            sum += big_pow(phi1, (seq_t)(d1 + 1));

            if (d1 != d2) {
                seq_t phi2 = phi(d2);
                sum += big_pow(phi2, (seq_t)(d2 + 1));
            }
        }
    }

    return sum;
}
