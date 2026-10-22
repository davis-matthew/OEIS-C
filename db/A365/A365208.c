/*
 * A365208  The number of divisors d of n such that gcd(d, n/d) is a 3-smooth
 *          number (A003586).
 * https://oeis.org/A365208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = (e2+1)(e3+1) * 2^k where e2 = exponent of 2 in n, e3 =
 *            exponent of 3 in n, k = number of distinct prime factors p ≥ 5
 *            dividing n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"   /* provides factor_u64 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* below offset -- undefined behaviour */

    uint64_t N = (uint64_t)n;
    unsigned e2 = 0, e3 = 0;

    /* exponent of 2 */
    while ((N & 1ULL) == 0) {
        ++e2;
        N >>= 1;
    }

    /* exponent of 3 */
    while (N % 3ULL == 0) {
        ++e3;
        N /= 3ULL;
    }

    /* factor remaining part to count distinct primes ≥5 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);

    size_t k = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] >= 5ULL) ++k;
    }

    /* compute (e2+1)(e3+1) * 2^k */
    seq_t result = (seq_t)(e2 + 1) * (seq_t)(e3 + 1);

    seq_t pow2 = 1;
    for (size_t i = 0; i < k; ++i) {
        pow2 *= 2;
    }

    result *= pow2;
    return result;
}
