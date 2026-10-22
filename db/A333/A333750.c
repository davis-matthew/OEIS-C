/*
 * A333750  Number of prime power divisors of n that are <= sqrt(n).
 * https://oeis.org/A333750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of prime power divisors of n that are <= sqrt(n)
 * Range:     u8 n <= 1296, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* sqrt(n) as an idx_t */
    idx_t sqrt_n = _floor_isqrt_u64(n);

    /* Factorise n: primes[i]^exps[i] */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t limit = (seq_t)sqrt_n;
        seq_t pow = p;          /* current p^k */

        for (size_t k = 0; k < exps[i]; ++k)
        {
            if (pow > limit)
                break;          /* further powers are larger */

            ++result;           /* count this prime power divisor */

            /* avoid overflow before next multiplication */
            if (pow > limit / p)
                break;
            pow = pow * p;
        }
    }

    return result;
}
