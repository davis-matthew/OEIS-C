/*
 * A390555  The number of exponential divisors of n^2.
 * https://oeis.org/A390555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e∥n} d(2e) where d(k) is the divisor function
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];
        uint64_t k_val = (uint64_t)(2 * e);   /* compute 2e as uint64_t */
        seq_t t = 1;
        uint64_t k = k_val;
        uint64_t p = 2;
        while (p * p <= k) {
            uint64_t c = 0;
            while (k % p == 0) {
                ++c;
                k /= p;
            }
            if (c > 0) {
                t *= (c + 1);
            }
            ++p;
        }
        if (k > 1) {
            t *= 2;
        }
        res *= t;
    }
    return res;
}
