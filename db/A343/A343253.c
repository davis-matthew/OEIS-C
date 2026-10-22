/*
 * A343253  a(n) is the least k0 <= n such that v_11(n), the 11-adic order of
 *          n, can be obtained by the formula: v_11(n) = log_11(n / L_11(k0,
 *          n)), where L_11(k0, n) is the lowest common denominator of the
 *          elements of the set S_11(k0, n) = {(1/n)*binomial(n, k), with 0 <
 *          k <= k0 such that k is not divisible...
 * https://oeis.org/A343253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest power of a prime different from 11 dividing n
 *            (equivalently, the greatest power of a prime ≠ 11 that divides
 *            n) For example: a(12) = 4 (since 12 = 2²·3 and 2² is the largest
 *            such power)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   cmp_seq_t, factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   // factor_u64(n, primes, exps, &cnt)
#include "../utils/pow_uint.h"    // pow_uint(base, exp)
#include "../utils/cmp_seq_t.h"   // cmp_seq_t(const void*, const void*)

/* Return the a(n) value as defined above. */
seq_t a(idx_t n) {
    if (n <= 1) {   /* n = 1 has no prime factors ≠ 11 */
        return (seq_t)1;
    }

    /* Factor n: primes[] contains distinct prime factors, exps[] the exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t best = (seq_t)1;          /* at least 1 */

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 11) {      /* ignore 11 */
            continue;
        }
        seq_t term = pow_uint(primes[i], (idx_t)exps[i]);   /* p^e */
        if (cmp_seq_t(&term, &best) > 0) {
            best = term;
        }
    }
    return best;
}
