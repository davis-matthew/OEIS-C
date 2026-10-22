/*
 * A382788  The sum of divisors of n that are numbers whose number of divisors
 *          is a power of 2.
 * https://oeis.org/A382788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of divisors d of n such that τ(d) is a power of 2. A
 *            divisor d satisfies the condition iff for each prime factor p^e
 *            of d, e+1 is a power of 2
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

static void rec(size_t idx, size_t total, seq_t cur,
                const uint64_t *primes, const uint64_t *exps,
                seq_t *sum)
{
    if (idx == total) {
        *sum += cur;
        return;
    }
    uint64_t base  = primes[idx];
    uint64_t maxe  = exps[idx];
    for (uint64_t e = 0; e <= maxe; ++e) {
        if (((e + 1) & e) == 0) {              // e+1 is a power of two
            seq_t pwr = pow_uint(base, e);
            rec(idx + 1, total, cur * pwr, primes, exps, sum);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset is 1, but guard against 0 */

    uint64_t nn      = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t sum = 0;
    rec(0, cnt, 1, primes, exps, &sum);
    return sum;
}
