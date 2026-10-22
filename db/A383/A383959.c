/*
 * A383959  The number of prime powers p^e having the property that e is a
 *          unitary divisor of the p-adic valuation of n.
 * https://oeis.org/A383959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p^k || n} 2^{ω(k)}, where ω(k) is the number of
 *            distinct prime factors of the exponent k. This is an additive
 *            function: a(p^k) = 2^{ω(k)}. The implementation factors n and
 *            sums the contributions
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t nn = (uint64_t)n;
    seq_t sum = 0;

    /* arrays are large enough for all distinct prime factors of nn */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(nn, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t temp = e;
        uint64_t omega = 0;

        for (uint64_t d = 2; d * d <= temp; ++d) {
            if (temp % d == 0) {
                ++omega;
                while (temp % d == 0)
                    temp /= d;
            }
        }
        if (temp > 1)
            ++omega;

        sum += ((seq_t)1 << omega);
    }

    return sum;
}
