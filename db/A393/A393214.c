/*
 * A393214  a(n) = d_4(n)^2, where d_4 is the 4th Piltz function (A007426).
 * https://oeis.org/A393214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = d_4(n)^2, where d_4(n) = ∏_p ( (e_p + 3) choose 3 ) and
 *            e_p is the exponent of prime p in the factorisation of n
 * Range:     u8 n <= 5, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
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
    /* d_4(1) = 1, so a(1) = 1 automatically. */
    size_t cnt;
    uint64_t primes[64];
    uint64_t exps[64];

    /* Factorise n.  The helper writes the distinct primes into primes[]
       and their exponents into exps[], returning the count in cnt. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t d4 = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        /* Binomial coefficient C(e+3,3) = (e+3)(e+2)(e+1)/6. */
        seq_t binom = ((seq_t)(e + 3) * (e + 2) * (e + 1)) / 6;
        d4 *= binom;
    }

    /* Return the square of d_4(n). */
    return d4 * d4;
}
