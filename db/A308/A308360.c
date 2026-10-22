/*
 * A308360  Product of positive divisors d of n that are divisible by every
 *          prime that divides n.
 * https://oeis.org/A308360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p^e||n} p^{ (∏ e_i) * (e+1)/2 }
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 143, u64 n <= 431,
 *            u128 n <= 2591, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where the product in the exponent is over all prime
 *   factors of n.  For squarefree n this simplifies to a(n)=n. */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    uint64_t uu = (uint64_t)n;
    if (uu <= 1) return (seq_t)1;          /* a(1)=1 and a(0) unused */

    /* factor n */
/*  the factor_u64 routine writes primes and exponents into the supplied arrays
 *  and returns the number of distinct prime factors in *cnt.                     */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(uu, primes, exps, &cnt);

    /* product of exponents */
    seq_t E = 1;
    for (size_t i = 0; i < cnt; ++i)
        E *= (seq_t)exps[i];

    /* compute the product of the required divisors */
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* exponent for this prime:  E * (e+1) / 2  */
        seq_t exp_factor = (E * (seq_t)(exps[i] + 1)) / 2;

        seq_t p_pow = big_pow((seq_t)primes[i], exp_factor);
        res *= p_pow;
    }

    return res;
}
