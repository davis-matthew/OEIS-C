/*
 * A344225  a(n) = Sum_{k=1..n} tau(gcd(k,n)^(n/gcd(k,n))), where tau(n) is
 *          the number of divisors of n.
 * https://oeis.org/A344225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} φ(n/d) * τ(d^(n/d)) (OEIS A344225)
 * Range:     u8 n <= 29, u16 n <= 245, u32 n <= 6989, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"            /* seq_t phi(idx_t m) */
#include "../utils/factor_u64.h"     /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset starts at 1 – caller should use n ≥ 1 */

    /* compute integer square root of n */
    idx_t lim = 0;
    while ((lim + 1) * (lim + 1) <= n)
        lim++;

    /* buffers for prime factors – 64 slots are enough for n up to 64-bit */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    seq_t sum = 0;

    for (idx_t i = 1; i <= lim; ++i) {
        if (n % i != 0)
            continue;

        /* first divisor d1 = i */
        idx_t d1 = i;
        idx_t k1 = n / d1;           /* exponent of d1 */
        seq_t phi_k1 = phi(k1);

        factor_u64(d1, primes, exps, &cnt);
        seq_t tau_d1k1 = 1;
        for (size_t j = 0; j < cnt; ++j) {
            seq_t term = (seq_t)(exps[j] * k1 + 1);
            tau_d1k1 *= term;
        }
        sum += phi_k1 * tau_d1k1;

        /* second divisor d2 = n / i, if distinct */
        if (i != n / i) {
            idx_t d2 = n / i;
            idx_t k2 = i;            /* exponent of d2 */
            seq_t phi_k2 = phi(k2);

            factor_u64(d2, primes, exps, &cnt);
            seq_t tau_d2k2 = 1;
            for (size_t j = 0; j < cnt; ++j) {
                seq_t term = (seq_t)(exps[j] * k2 + 1);
                tau_d2k2 *= term;
            }
            sum += phi_k2 * tau_d2k2;
        }
    }

    return sum;
}
