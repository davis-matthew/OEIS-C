/*
 * A361264  Multiplicative with a(p^e) = p^(e + 2), e > 0.
 * https://oeis.org/A361264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * (Π_{p|n} p)^2 (multiplicative with a(p^e)=p^(e+2) for
 *            e>0)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
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
    if (n == 1)
        return 1;                           /* a(1) = 1 */

    uint64_t primes[64];                    /* enough for n <= 2^64-1 */
    uint64_t exps[64];
    size_t   cnt;

    factor_u64(n, primes, exps, &cnt);       /* factorize n */

    seq_t rad = 1;                           /* product of distinct primes */
    for (size_t i = 0; i < cnt; ++i)
        rad *= (seq_t)primes[i];

    seq_t rad_sq = rad * rad;
    return (seq_t)n * rad_sq;                /* a(n) = n * rad^2 */
}
