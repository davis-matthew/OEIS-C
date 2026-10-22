/*
 * A338164  Dirichlet g.f.: (zeta(s-2) / zeta(s))^2.
 * https://oeis.org/A338164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = multiplicative with a(p^e) = p^(2e - 4) * (p^4 +
 *            e*(p^2-1)^2 - 1)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 481, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];

        seq_t p2 = p * p;                /* p^2 */
        seq_t p4 = p2 * p2;              /* p^4 */
        seq_t sq = (p2 - (seq_t)1) * (p2 - (seq_t)1); /* (p^2-1)^2 */
        seq_t inner = p4 + e * sq - (seq_t)1;         /* inner part */

        int64_t diff = (int64_t)2 * (int64_t)e - 4;   /* 2e-4 */
        seq_t val;
        if (diff >= 0)
        {
            seq_t pow_part = big_pow(p, (seq_t)diff);
            val = pow_part * inner;
        }
        else
        {
            seq_t divisor = big_pow(p, (seq_t)(-diff));
            val = inner / divisor;
        }

        result *= val;
    }
    return result;
}
