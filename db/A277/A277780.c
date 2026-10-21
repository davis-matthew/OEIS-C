/*
 * A277780  a(n) is the least k > n such that n*k^2 is a cube.
 * https://oeis.org/A277780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k > n such that n*k^2 is a cube. Decompose n = q *
 *            m^3 with q cubefree. Then a(n) = q*(m+1)^3
 * Range:     u8 n <= 32, u16 n <= 8192, u32 n <= 10000, u64 n <= 10000,
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
    if (n == 0) return (seq_t)0;  /* undefined for 0, but harmless */

    /* Factor n using the provided factor_u64 utility. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t len = 64;
    factor_u64((uint64_t)n, primes, exps, &len);  /* len -> number of distinct primes */

    seq_t m = (seq_t)1;   /* cube part */
    seq_t q = (seq_t)1;   /* cubefree part */

    for (size_t i = 0; i < len; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        uint64_t e3  = e / 3;     /* exponent for the cube part */
        uint64_t e2  = e % 3;     /* exponent for the cubefree part */

        /* multiply m by p^e3 */
        for (uint64_t j = 0; j < e3; ++j)
            m *= (seq_t)p;

        /* multiply q by p^e2 (only 0,1,2) */
        if (e2 == 1) {
            q *= (seq_t)p;
        } else if (e2 == 2) {
            seq_t pp = (seq_t)p * (seq_t)p;
            q *= pp;
        }
    }

    /* compute (m+1)^3 */
    seq_t mp1 = m + (seq_t)1;
    seq_t mp1_sq = mp1 * mp1;
    seq_t mp1_cu = mp1_sq * mp1;

    /* answer = q * (m+1)^3 */
    return q * mp1_cu;
}
