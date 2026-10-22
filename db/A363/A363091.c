/*
 * A363091  Sum of the divisor complements of the refactorable unitary
 *          divisors of n.
 * https://oeis.org/A363091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n, gcd(d, n/d)=1, tau(d)|d} n/d
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor n into primes and exponents.
       Arrays are of maximum size 64, which is more than enough
       for all practical inputs (n <= 2^64).                               */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, pr, ex, &pcnt);

    seq_t sum = 0;

    /* Enumerate all unitary divisors using subsets of prime factors. */
    idx_t mask_limit = 1ULL << pcnt;        /* 2^pcnt, safe for pcnt <= 64 */
    for (idx_t mask = 0; mask < mask_limit; ++mask)
    {
        seq_t d   = 1;        /* current divisor */
        seq_t tau = 1;        /* divisor count of d */

        for (size_t i = 0; i < pcnt; ++i)
        {
            if ((mask >> i) & 1ULL)
            {
                /* multiply d by p_i ^ e_i */
                seq_t term = 1;
                for (uint64_t e = 0; e < ex[i]; ++e)
                    term *= pr[i];

                d   *= term;
                tau *= (seq_t)(ex[i] + 1);
            }
        }

        /* d is a unitary divisor of n by construction.
           Check the refactorable condition tau(d) | d. */
        if (d % tau == 0)
        {
            sum += (seq_t)n / d;   /* add the complement n/d */
        }
    }

    return sum;
}
