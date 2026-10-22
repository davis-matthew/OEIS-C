/*
 * A400146  The sum of divisors d of n such that gcid(d, n/d) is a power of 2,
 *          where gcid is the greatest common infinitary divisor.
 * https://oeis.org/A400146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e∥n} f(p^e) where f(2^e) = 2^(e+1) – 1 and for odd
 *            p, f(p^e) = Σ_{i=0..e} (C(e,i) mod 2) · p^i (C(e,i) odd iff (i &
 *            ~e) == 0)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    if (n == 0)                /* offset is 1, but guard anyway */
        return 0;

    /* Factor n – generous buffer for ≤64‑bit numbers */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t idx = 0; idx < cnt; ++idx)
    {
        uint64_t p = primes[idx];
        uint64_t e = exps[idx];

        seq_t term;

        if (p == 2)
        {
            /* 2^(e+1) – 1 */
            seq_t pow2 = 1;
            for (uint64_t j = 0; j < e; ++j)
                pow2 <<= 1;                  /* pow2 = 2^e */
            term = (pow2 << 1) - 1;          /* 2^(e+1) – 1 */
        }
        else
        {
            /* Sum over i with binomial(e,i) odd */
            seq_t sum  = 1;                /* i = 0 */
            seq_t p_pow = 1;               /* p^(i-1) */
            for (uint64_t i = 1; i <= e; ++i)
            {
                p_pow *= (seq_t)p;          /* p^i */
                if ((i & ~e) == 0)          /* binomial(e,i) odd */
                    sum += p_pow;
            }
            term = sum;
        }

        result *= term;
    }

    return result;
}
