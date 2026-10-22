/*
 * A360522  a(n) = Sum_{d|n} Max({d'; d'|n, gcd(d, d') = 1}).
 * https://oeis.org/A360522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e||n} (p^e + e) (multiplicative)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; an argument of 0 is treated
       as 0 to avoid undefined behaviour. */
    if (n == 0)
        return (seq_t)0;

    /* Factorise n.  The helper stores the primes in primes[] and
       their exponents in exps[].  The number of distinct primes is
       returned in cnt. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the product of (p^e + e) for each prime power. */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute p^e in the target type seq_t.  For small seq_t
           sizes the product may overflow; this is acceptable because
           such values cannot be represented anyway. */
        seq_t pe = (seq_t)1;
        for (uint64_t j = 0; j < e; ++j)
            pe *= (seq_t)p;

        result *= (pe + (seq_t)e);
    }

    return result;   /* For n = 1, cnt = 0 → result = 1 */
}
