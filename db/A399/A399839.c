/*
 * A399839  The sum of divisors d of n such that gcud(d, n/d) is a power of 2,
 *          where gcud is the greatest common unitary divisor.
 * https://oeis.org/A399839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) is multiplicative. If n = ∏ p_i^{e_i} then * for p = 2 :
 *            S(2,e) = 2^{e+1} - 1 * for p odd : if e is odd : S(p,e) =
 *            (p^{e+1} - 1) / (p - 1) if e is even: S(p,e) = (p^{e+1} - 1) /
 *            (p - 1) – p^{e/2} a(n) = ∏ S(p_i, e_i)
 * Range:     u8 n <= 16, u16 n <= 2778, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t sum;

        if (p == 2) {
            /* S(2,e) = 2^{e+1} - 1 */
            sum = pow2((idx_t)(e + 1)) - (seq_t)1;
        } else {
            /* S(p,e) = (p^{e+1} - 1)/(p - 1)  (minus p^{e/2} if e even) */
            seq_t pseq = (seq_t)p;
            seq_t t   = pow_int(pseq, (idx_t)(e + 1));   /* p^{e+1} */
            seq_t den = pseq - (seq_t)1;                 /* p - 1 */
            sum = (t - (seq_t)1) / den;

            if ((e & 1) == 0) {                       /* e even */
                seq_t sub = pow_int(pseq, (idx_t)(e / 2));
                sum -= sub;
            }
        }

        result *= sum;
    }

    return result;
}
