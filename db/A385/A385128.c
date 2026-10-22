/*
 * A385128  The number of divisors of n whose maximum exponent in their prime
 *          factorization is even.
 * https://oeis.org/A385128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n whose maximum prime exponent in
 *            the factorization of d is even
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void enumerate_divisors(int idx, int current_max, const uint64_t *exponents,
                               int num_primes, seq_t *result)
{
    if (idx == num_primes) {
        if ((current_max & 1) == 0) ++(*result);
        return;
    }
    /* For each possible exponent of the current prime (0 … exponents[idx]) */
    for (int e = 0; e <= (int)exponents[idx]; ++e) {
        int new_max = e > current_max ? e : current_max;
        enumerate_divisors(idx + 1, new_max, exponents, num_primes, result);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but guarded */

    uint64_t primes[64];
    uint64_t exps  [64];
    size_t  cnt    = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t count = 0;
    enumerate_divisors(0, 0, exps, (int)cnt, &count);
    return count;
}
