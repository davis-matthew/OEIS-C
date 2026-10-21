/*
 * A278509  a(n) = 3^{number of primes congruent to 3 modulo 4 dividing n
 *          (with multiplicity)}.
 * https://oeis.org/A278509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 3^{# of primes congruent to 3 modulo 4 dividing n (with
 *            multiplicity)}
 * Range:     u8 n <= 728, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n <= 1)
        return (seq_t)1;

    /* Factor n.  We need a small buffer for the prime factors and exponents.
     * The maximum number of distinct prime factors of a 64‑bit integer is bounded
     * by log2(n) < 64, so an array of 64 is safe for all idx_t values. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Count the total exponent of primes ≡ 3 (mod 4). */
    size_t exp_total = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (primes[i] % 4 == 3)
            exp_total += (size_t)exps[i];
    }

    /* 3^exp_total is the desired value.  pow3 expects an idx_t exponent. */
    return pow3((idx_t)exp_total);
}
