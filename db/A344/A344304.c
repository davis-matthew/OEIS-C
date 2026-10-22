/*
 * A344304  Number of cyclic subgroups of the group (C_n)^8, where C_n is the
 *          cyclic group of order n.
 * https://oeis.org/A344304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of cyclic subgroups of (C_n)^8 multiplicative:
 *            a(p^e) = 1 + ((p^8-1)/(p-1))*((p^(7e)-1)/(p^7-1))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 479,
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
    if (n <= 1)
        return (seq_t)1;

    uint64_t primes[64], exps[64];
    size_t pc = 0;
    factor_u64((uint64_t)n, primes, exps, &pc);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < pc; ++i)
    {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* term1 = (p^8-1)/(p-1) = 1 + p + ... + p^7 */
        seq_t term1 = 1;
        seq_t pow = 1;
        for (size_t j = 0; j < 7; ++j)
        {
            pow *= p;
            term1 += pow;
        }

        /* term2 = (p^(7e)-1)/(p^7-1) = 1 + p^7 + p^14 + ... + p^(7*(e-1)) */
        seq_t term2 = 1;
        if (e > 1)
        {
            seq_t p7 = 1;
            for (size_t k = 0; k < 7; ++k)
                p7 *= p;              /* p^7 */

            seq_t curr = 1;
            for (uint64_t k = 1; k < e; ++k)
            {
                curr *= p7;
                term2 += curr;
            }
        }

        seq_t contrib = term1 * term2 + (seq_t)1;
        result *= contrib;
    }

    return result;
}
