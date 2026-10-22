/*
 * A351563  a(n) is the exponent of the second smallest prime factor of n, or
 *          0 if n is a power of a prime.
 * https://oeis.org/A351563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of the second smallest prime factor of n, or 0
 *            if n has at most one distinct prime factor (i.e., n is a prime
 *            power)
 * Range:     u8 n <= 9452, u16 n <= 9452, u32 n <= 9452, u64 n <= 9452,
 *            u128 n <= 9452, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Maximum 64 distinct prime factors for a 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 fills `primes` and `exps` with the distinct primes and
       their exponents in ascending order, storing the number of distinct
       primes in `cnt`. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If there are fewer than two distinct prime factors, return 0. */
    if (cnt < 2)
        return 0;

    /* The second element of the exponent array corresponds to the second
       smallest prime factor's exponent. */
    return (seq_t)exps[1];
}
