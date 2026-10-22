/*
 * A394470  a(n) = n*tau(n) - (n-1)*Sum_{p|n, p prime} [gcd(p,n/p) = 1], where
 *          [ ] is the Iverson bracket.
 * https://oeis.org/A394470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*tau(n) - (n-1)*cnt1(n) where cnt1(n) = #{p | n :
 *            exponent of p in n equals 1}
 * Range:     u8 n <= 35, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* factorization buffer (large enough for n ≤ 10⁴) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t tau = 1;          /* divisor count */
    seq_t cnt1 = 0;         /* primes with exponent 1 */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)(exps[i] + 1);
        tau = tau * e;          /* multiply by (exponent+1) */
        if (exps[i] == 1) {
            cnt1 = cnt1 + (seq_t)1;
        }
    }

    seq_t n_seq = (seq_t)n;
    seq_t result = n_seq * tau - (n_seq - 1) * cnt1;
    return result;
}
