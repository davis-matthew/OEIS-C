/*
 * A349258  a(n) is the number of prime powers (not including 1) that are
 *          infinitary divisors of n.
 * https://oeis.org/A349258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{p^e || n} (2^popcount(e) - 1) // Number of prime
 *            powers (excluding 1) that are infinitary divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    if (n < 2)
        return (seq_t)0;   /* a(1) = 0, undefined for n=0 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* factor_u64 returns the prime factors and exponents of n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* exponent of the i-th prime factor */
        unsigned long long e = (unsigned long long)exps[i];
        /* number of 1-bits in the binary representation of e */
        unsigned int bits = __builtin_popcountll(e);
        /* 2^bits - 1 */
        seq_t term = ((seq_t)1 << bits) - 1;
        sum += term;
    }
    return sum;
}
