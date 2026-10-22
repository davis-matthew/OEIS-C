/*
 * A335603  a(n) = p*q where p is the sequential number (or PrimePi, A000720)
 *          of the largest prime divisor of n, and q is the maximal exponent
 *          in the canonical representation of n (A051903).
 * https://oeis.org/A335603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime_pi(largest_prime_factor(n)) *
 *            maximal_exponent_in_factorization(n)
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1)=0 */

    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(m, primes, exps, &cnt);

    uint64_t maxexp = 0;
    uint64_t largestPrime = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (exps[i] > maxexp) maxexp = exps[i];
        if (primes[i] > largestPrime) largestPrime = primes[i];
    }

    seq_t p = 0;
    if (largestPrime >= 2)
        p = prime_pi((idx_t)largestPrime);

    return p * (seq_t)maxexp;
}
