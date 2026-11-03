/*
 * A355937  a(n) = 1 if the number of divisors of n is a noncomposite,
 *          otherwise 0.
 * https://oeis.org/A355937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the number of divisors of n is a noncomposite (i.e.
 *            1 or a prime), otherwise 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9264 (2026-11-01)
 * Helpers:   factor_u64, is_prime_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_i32.h"

seq_t a(idx_t n)
{
    /* Compute the divisor count τ(n) */
    uint64_t nn = (uint64_t)n;
    /* Temporary storage for prime factors and exponents.
       n has at most 64 distinct prime factors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    uint64_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (exps[i] + 1);

    /* τ(n) is a noncomposite iff it equals 1 or is prime.   */
    seq_t result = 0;
    if (tau == 1 || is_prime_i32(tau))
        result = 1;

    return result;
}
