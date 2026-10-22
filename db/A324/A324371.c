/*
 * A324371  Product of all primes p dividing n such that the sum of the base p
 *          digits of n is less than p, or 1 if no such prime.
 * https://oeis.org/A324371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   Product of all primes p dividing n such that the sum of the base
 *            p digits of n is less than p, or 1 if no such prime
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    if (n < 1) return (seq_t)0;             /* not expected, but safe */

    uint64_t nn = (uint64_t)n;

    /* Buffers large enough to hold all distinct prime factors of a 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];

        /* Compute sum of the digits of nn in base p. */
        uint64_t tmp = nn;
        uint64_t sum = 0;
        while (tmp > 0) {
            sum += tmp % p;
            tmp /= p;
        }

        if (sum < p) {
            result *= (seq_t)p;   /* multiply only once per prime factor */
        }
    }
    return result;
}
