/*
 * A374537  a(n) is the sum of the squares of the divisors of n that are
 *          exponentially odd numbers.
 * https://oeis.org/A374537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of the squares of the divisors of n that are
 *            exponentially odd numbers. a(p^e) = 1 + p^2 * (p^(4*⌊(e-1)/2⌋+4)
 *            - 1) / (p^4 - 1)
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
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
    if (n == 0) /* undefined, guard against under‑flow */
        return 0;

    uint64_t nn = (uint64_t)n;

    /* The number of distinct prime factors of any 64‑bit integer
       is at most 64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;

    factor_u64(nn, primes, exps, &pcnt);

    seq_t result = 1; /* product of a(p^e) over all prime powers */

    for (size_t i = 0; i < pcnt; ++i)
    {
        seq_t p    = (seq_t)primes[i];
        uint64_t e = exps[i];

        seq_t term  = 1; /* 1 + sum_{odd k ≤ e} p^(2k) */
        seq_t power = 1; /* p^0 */

        for (uint64_t k = 1; k <= e; ++k)
        {
            power *= p;           /* now power = p^k */

            if ((k & 1) == 1)     /* k odd */
                term += power * power;
        }

        result *= term;
    }

    return result;
}
