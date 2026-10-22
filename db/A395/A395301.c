/*
 * A395301  a(n) = tau(n) - omega(n) + Sum_{p|n, p prime} gcd(p,n/p).
 * https://oeis.org/A395301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n) - omega(n) + Σ_{p|n} gcd(p, n/p) τ = divisor
 *            count, ω = number of distinct primes
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* provides factor_u64() */

seq_t a(idx_t n)
{
    /* handle n = 0 undefined; return 0 for safety */
    if (n == 0)
        return 0;

    /* factor the integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* τ(n) = product of (exponent + 1) */
    seq_t tau = 1;
    /* Σ_{p|n} gcd(p, n/p) */
    seq_t sum = 0;

    for (size_t i = 0; i < cnt; ++i)
    {
        unsigned int e = (unsigned int)exps[i];
        tau = tau * (seq_t)(e + 1);

        if (e >= 2)
            sum += (seq_t)primes[i];
        else
            sum += (seq_t)1;
    }

    seq_t omega = (seq_t)cnt;
    return tau - omega + sum;
}
