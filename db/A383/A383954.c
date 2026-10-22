/*
 * A383954  a(n) = Product_{i} (phi(p_i^e_i)-1) where n = Product_{i} p_i^e_i
 *          and phi is the Euler phi function.
 * https://oeis.org/A383954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Product_{p^e || n} (phi(p^e)-1) Multiplicative: a(p^e) =
 *            (p-1)*p^(e-1)-1, a(1)=1
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    if (n == 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* compute p^(e-1) */
        seq_t pow_p = (seq_t)1;
        for (size_t k = 1; k < e; ++k)
            pow_p *= (seq_t)p;

        seq_t term = ((seq_t)(p - 1)) * pow_p - 1;
        result *= term;
    }

    return result;
}
