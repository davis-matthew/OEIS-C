/*
 * A279912  a(n) = Sum_{i=1..n} denominator(i^n/n).
 * https://oeis.org/A279912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d | n} d * phi(n/d)^2 Multiplicative: a(p^k) =
 *            p^(k-1) * ((p-1)*p^k + 1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* seq_t and idx_t are defined by the build system */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* a(0) = 0 */

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        unsigned int k = (unsigned int)exps[i];

        /* p^k */
        seq_t pow_pk = (seq_t)1;
        for (unsigned int t = 0; t < k; ++t) pow_pk *= p;

        /* p^(k-1) (for k >= 1) */
        seq_t pow_pk_minus1 = (seq_t)1;
        if (k > 0) {
            for (unsigned int t = 0; t < k - 1; ++t) pow_pk_minus1 *= p;
        }

        /* inner = (p-1)*p^k + 1 */
        seq_t inner = ((p - (seq_t)1) * pow_pk) + (seq_t)1;

        /* term for this prime power */
        seq_t term = pow_pk_minus1 * inner;

        /* accumulate multiplicatively */
        result *= term;
    }
    return result;
}
