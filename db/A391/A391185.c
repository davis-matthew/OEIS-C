/*
 * A391185  The sum of the divisors of n that are of the form x^2 + 2*y^2
 *          (A002479).
 * https://oeis.org/A391185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} A391183(d) * d
 * Range:     u8 n <= 16, u16 n <= 1058, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented using the multiplicative formula:
   a(p^e) = (p^(2*(floor(e/2)+1)) - 1) / (p^2 - 1)  if p ≡ 5 or 7 (mod 8)
   a(p^e) = (p^(e+1) - 1) / (p - 1)                 otherwise
   and a(n) = ∏ a(p^e).  */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not used, but keeps the function well‑defined */

    /* Factor n.  A 64‑bit integer can have at most 64 prime factors (counting multiplicities). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        idx_t    e   = (idx_t)exps[i];
        seq_t    val;

        if ((p & 7ULL) == 5 || (p & 7ULL) == 7) {          /* p ≡ 5 or 7 (mod 8) */
            idx_t k   = e / 2 + 1;                         /* floor(e/2)+1 */
            seq_t num = pow_uint(p, 2 * k) - 1;
            seq_t den = pow_uint(p, 2) - 1;
            val      = num / den;                        /* exact division */
        } else {                                          /* other primes */
            seq_t num = pow_uint(p, e + 1) - 1;
            seq_t den = pow_uint(p, 1) - 1;               /* p - 1 */
            val      = num / den;
        }
        result *= val;
    }

    return result;
}
