/*
 * A349281  a(n) is the number of prime powers (not including 1) that are
 *          (1+e)-divisors of n.
 * https://oeis.org/A349281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p^e‖n} τ(e) where τ(e) is the number of positive
 *            divisors of the exponent e
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _tau_small, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/_tau_small.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0, and a(0) is unused in the sequence */
        return 0;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* Factorize n into prime powers p^e */
    factor_u64(nn, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += _tau_small(exps[i]);

    return sum;
}
