/*
 * A308056  a(1) = 1, a(n) is the sum of the divisors d of n such that d and n
 *          are exponentially coprime.
 * https://oeis.org/A308056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A308056
    a(1) = 1
    a(n) = sum of divisors d of n such that, for every prime p | n,
           the exponent of p in d is coprime to the exponent of p in n.
    Formula: multiplicative, a(p^e) = Σ_{i=1..e, gcd(i,e)=1} p^i
*/

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */

/* Factorization routine */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr,
                                      uint64_t *ex, size_t *cnt); */

/* Integer gcd routine */
#include "../utils/gcd_int.h"   /* int gcd_int(int a, int b); */

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1;

    /* arrays for prime factors and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* factor n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t idx = 0; idx < cnt; ++idx)
    {
        seq_t p = (seq_t)primes[idx];
        int   e = (int)exps[idx];

        seq_t sum   = (seq_t)0;
        seq_t power = (seq_t)1;   /* will become p^i */

        for (int i = 1; i <= e; ++i)
        {
            power *= p;            /* p^i */
            if (gcd_int(i, e) == 1)
                sum += power;
        }

        result *= sum;
    }

    return result;
}
