/*
 * A370239  The sum of divisors of n that are squares of squarefree numbers.
 * https://oeis.org/A370239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n, e≥2} (1 + p^2) where p^e || n means p^e
 *            divides n and p^(e+1) does not
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* not used, offset is 1 */
    seq_t result = (seq_t)1;
    if (n == 1) return result;             /* n=1 has no prime factors */

    /* arrays for prime factors and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] >= 2) {
            seq_t p = (seq_t)primes[i];
            seq_t p2 = p * p;            /* p squared */
            result *= (p2 + (seq_t)1);   /* multiply by 1 + p^2 */
        }
    }
    return result;
}
