/*
 * A390556  The sum of the exponential divisors of n^2.
 * https://oeis.org/A390556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = A051377(n^2) – the sum of exponential divisors of n²
 * Range:     u8 n <= 11, u16 n <= 185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used for OEIS offset 1 */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        idx_t    e   = (idx_t)exps[i];
        idx_t    k   = 2 * e;               /* exponent in n² */
        seq_t    sum = 0;

        idx_t r = (idx_t)_floor_isqrt_u64(k);
        for (idx_t d = 1; d <= r; ++d) {
            if (k % d == 0) {
                seq_t term1 = pow_uint(p, d);
                sum += term1;
                idx_t d2 = k / d;
                if (d2 != d) {
                    seq_t term2 = pow_uint(p, d2);
                    sum += term2;
                }
            }
        }
        result *= sum;
    }
    return result;
}
