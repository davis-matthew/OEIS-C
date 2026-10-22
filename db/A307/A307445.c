/*
 * A307445  Dirichlet g.f.: 1 / (zeta(s) * zeta(2*s)).
 * https://oeis.org/A307445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A307445: multiplicative function with
   a(p^e) = -1  for e = 1,2
   a(p^e) =  1  for e = 3
   a(p^e) =  0  for e >= 4
   a(1) = 1
*/
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined below offset */
    if (n == 1) return 1;          /* base case */

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        if (e >= 4)          /* contribution is zero */
            return 0;
        if (e == 1 || e == 2)
            result = -result; /* multiply by -1 */
        /* e == 3 : multiply by +1, no change */
    }
    return result;
}
