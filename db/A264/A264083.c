/*
 * A264083  Number of orthogonal 3 X 3 matrices over the ring Z/nZ.
 * https://oeis.org/A264083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(p,e) where f(2,1) = 6, f(2,2) = 384, f(2,e) =
 *            12 * 8^e (e ≥ 3), f(p,e) = 2 * p * (p^2 – 1) * p^(3e–3) (p odd
 *            prime, e ≥ 1). The function multiplies the contributions of
 *            prime powers
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 124, u64 n <= 124,
 *            u128 n <= 124, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* result initialized to 1 */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];
        seq_t val;

        if (p == 2) {
            if (e == 1) {
                val = (seq_t)6;
            } else if (e == 2) {
                val = (seq_t)384;
            } else {
                seq_t eight_e = seq_pow((seq_t)8, e);
                val = seq_mul((seq_t)12, eight_e);
            }
        } else {
            /* odd prime case: a(p^e) = 2*p*(p^2-1)*p^(3e-3) */
            seq_t two_p = seq_mul((seq_t)2, p);
            seq_t p_sq = seq_mul(p, p);
            p_sq -= (seq_t)1;                 /* p^2 - 1 */
            seq_t base  = seq_mul(two_p, p_sq); /* 2*p*(p^2-1) */

            if (e > 1) {
                idx_t exp = 3 * (e - 1);     /* exponent for p^(3e-3) */
                seq_t pow = seq_pow(p, exp);
                val = seq_mul(base, pow);
            } else {
                val = base;
            }
        }

        result = seq_mul(result, val);
    }

    return result;
}
