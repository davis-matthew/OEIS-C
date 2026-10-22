/*
 * A324369  Product of all primes p dividing n such that the sum of the base p
 *          digits of n is at least p, or 1 if no such prime.
 * https://oeis.org/A324369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = product of all primes p dividing n such that the sum of
 *            the base‑p digits of n is at least p (a(1) = 1)
 * Range:     u8 n <= 560, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t num = (uint64_t)n;
    if (num <= 1) {
        return (seq_t)1;
    }

    /* factor_u64 returns the distinct prime factors of num in primes[].
       The maximum number of distinct primes that fit in 64 bits is < 64. */
    uint64_t primes[64];
    uint64_t exps[64];      /* exps are unused but required by the helper */
    size_t cnt = 0;
    factor_u64(num, primes, exps, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];

        /* Sum of the base‑p digits of num. */
        uint64_t sum = 0;
        uint64_t tmp = num;
        while (tmp) {
            sum += tmp % p;
            tmp /= p;
        }

        if (sum >= p) {
            prod = prod * (seq_t)p;
        }
    }
    return prod;
}
