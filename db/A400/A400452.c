/*
 * A400452  The number of divisors d of n such that gcd(d, n/d) is a prime
 *          power (A246655).
 * https://oeis.org/A400452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = Σ_{p|n} (exp_p − 1) * 2^{k−1}, where k is the number of
 *            distinct prime factors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(temp, primes, exps, &cnt);
    if (cnt == 0) return 0;
    int k = (int)cnt;
    seq_t pow2k1 = 1;
    for (int i = 0; i < k - 1; ++i) pow2k1 <<= 1;
    seq_t res = 0;
    for (size_t j = 0; j < cnt; ++j)
    {
        if (exps[j] > 1)
            res += (seq_t)(exps[j] - 1) * pow2k1;
    }
    return res;
}
