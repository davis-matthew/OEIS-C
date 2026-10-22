/*
 * A345446  Number of semiprime divisors of n whose square does not divide n.
 * https://oeis.org/A345446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of semiprime divisors d of n with d²∤n, i.e. a(n)
 *            = |{ d | d|n, Ω(d)=2, n mod d² ≠ 0 }|
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* use prime factorisation of n */
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* factorise n:  primes[0..cnt-1], exps[0..cnt-1] */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 0;

    /* count semiprimes of the form p² with exponent 2 or 3 */
    for (size_t i = 0; i < cnt; ++i)
    {
        if (exps[i] >= 2 && exps[i] <= 3)
            ++res;
    }

    /* count semiprimes of the form p*q with distinct primes, where at least
       one exponent is 1 (so that (p*q)² does not divide n) */
    for (size_t i = 0; i < cnt; ++i)
    {
        for (size_t j = i + 1; j < cnt; ++j)
        {
            /* both primes must divide n */
            if (exps[i] >= 1 && exps[j] >= 1)
            {
                /* exclude the case exps[i]≥2 and exps[j]≥2, because then
                   (p*q)² would divide n */
                if (!(exps[i] >= 2 && exps[j] >= 2))
                    ++res;
            }
        }
    }

    return res;
}
