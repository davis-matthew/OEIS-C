/*
 * A338563  a(n) = lcm(n, tau(n), sigma(n)) where tau(k) is the number of
 *          divisors of k (A000005) and sigma(k) is the sum of divisors of k
 *          (A000203).
 * https://oeis.org/A338563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, tau(n), sigma(n))
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"       /* gcd(seq_t, seq_t) */
#include "../utils/factor_u64.h"/* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/pow_uint.h"  /* pow_uint(uint64_t, idx_t) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 */
    if (n < 1) return 0;

    seq_t N = (seq_t)n;

    /* Factor n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    /* tau(n) = product (ex[i] + 1) */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau = tau * ((seq_t)(ex[i] + 1));

    /* sigma(n) = product (p^(e+1)-1)/(p-1) */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        seq_t num = pow_uint(pr[i], ex[i] + 1) - 1;
        seq_t denom = p - 1;
        seq_t term = num / denom;          /* division is exact */
        sigma = sigma * term;
    }

    /* lcm of first two */
    seq_t g1 = gcd(N, tau);
    seq_t lcm12 = (N / g1) * tau;

    /* lcm of all three */
    seq_t g2 = gcd(lcm12, sigma);
    seq_t result = (lcm12 / g2) * sigma;

    return result;
}
