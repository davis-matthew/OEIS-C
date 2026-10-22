/*
 * A381203  a(n) is the lcm of the elements of the set of bases and exponents
 *          in the prime factorization of n.
 * https://oeis.org/A381203
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = lcm of the bases and exponents in n's prime factorisation. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"          /* gcd(seq_t, seq_t) */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 1;          /* behaviour below offset is unspecified */

    uint64_t nn = (uint64_t)n;

    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        result = lcm_seq(result, p);
        result = lcm_seq(result, e);
    }

    return result;
}
