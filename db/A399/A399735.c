/*
 * A399735  The sum of the unitary divisors d of n such that n/d is a square.
 * https://oeis.org/A399735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = \prod_{p^e \parallel n} (e \text{ odd} ? p^e : p^e + 1)
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A399735 (sum of unitary divisors d of n such that n/d is a square). */

#include <stddef.h>            /* for size_t */
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Compute a(n) for n ≥ 1. For n = 0 return 0 (undefined but harmless). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* 64‑bit numbers need at most 16 distinct prime factors. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t  cnt = 0;

    /* factor_u64 fills primes[], exps[] and sets cnt. */
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        /* p^e as a seq_t (may be arbitrary precision). */
        seq_t p_pow = pow_uint(primes[i], (idx_t)exps[i]);

        /* Contribution: p^e if exponent odd, else p^e + 1. */
        if (exps[i] & 1U)
            result *= p_pow;
        else
            result *= (p_pow + 1);
    }

    return result;
}
