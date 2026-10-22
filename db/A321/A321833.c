/*
 * A321833  a(n) = Sum_{d|n, n/d==1 mod 4} d^9 - Sum_{d|n, n/d==3 mod 4} d^9.
 * https://oeis.org/A321833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n, n/d≡1 (mod 4)} d^9 – Sum_{d|n, n/d≡3 (mod 4)}
 *            d^9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"    /* factor_u64 */
#include "../utils/pow_uint.h"     /* pow_uint */

/* Return the n‑th term of OEIS A321833. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined, guard */
        return (seq_t)0;

    uint64_t nn      = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;   /* multiplicative identity */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t   factor_val;

        if (p == 2) {                     /* a(2ᵐ) = 512ᵐ */
            seq_t t = 1;
            for (uint64_t j = 0; j < e; ++j)
                t *= (seq_t)512;          /* 2⁹ = 512 */
            factor_val = t;
        }
        else if (p % 4 == 1) {            /* geometric series of p⁹ */
            seq_t p9   = pow_uint(p, 9);
            seq_t term = 1;
            seq_t sum  = 1;
            for (uint64_t j = 1; j <= e; ++j) {
                term *= p9;
                sum  += term;
            }
            factor_val = sum;
        }
        else {                            /* p ≡ 3 (mod 4) : alternating signs */
            seq_t p9      = pow_uint(p, 9);
            seq_t term    = 1;
            seq_t pos_sum = 1;   /* k = 0 contributes positively */
            seq_t neg_sum = 0;
            for (uint64_t j = 1; j <= e; ++j) {
                term *= p9;
                if ((j & 1) == 0)   /* even k */
                    pos_sum += term;
                else
                    neg_sum += term;
            }
            if ((e & 1) == 0)      /* e even */
                factor_val = pos_sum - neg_sum;
            else                   /* e odd, flip the sign */
                factor_val = neg_sum - pos_sum;
        }

        result *= factor_val;
    }

    return result;
}
