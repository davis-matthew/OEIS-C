/*
 * A385200  The sum of the exponents e for the integers k from 1 to n such
 *          that the greatest divisor of k that is a unitary divisor of n is a
 *          prime power p^e.
 * https://oeis.org/A385200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = uphi(n) * Σ_{p^e||n} e/(p^e-1) * where uphi(n) =
 *            Π_{p^e||n} (p^e-1) * * Compute using prime factorisation, then
 *            multiply the denominator * products (uphi) and sum the terms
 *            uphi/(p^e-1) * e. * This always yields an integer result
 * Range:     u8 n <= 443, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* special case n = 1 */
    if (n <= 1)
        return 0;

    /* Max number of distinct prime factors for 64‑bit n is < 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;

    /* factorise n */
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* store each denominator p^e-1 and compute uphi = Π denom */
    seq_t denom[64];
    seq_t uphi = 1;

    for (size_t i = 0; i < pcnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        seq_t pe = big_pow(p, e);     /* p^e */
        denom[i] = pe - 1;            /* p^e - 1 */
        uphi *= denom[i];
    }

    /* sum uphi/(p^e-1) * e over all prime powers */
    seq_t result = 0;
    for (size_t i = 0; i < pcnt; ++i)
    {
        seq_t term = uphi / denom[i];      /* exact division */
        term *= (seq_t)exps[i];
        result += term;
    }
    return result;
}
