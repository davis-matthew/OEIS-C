/*
 * A296081  a(n) = gcd(tau(n)-1, sigma(n)-1), where tau = A000005 and sigma =
 *          A000203.
 * https://oeis.org/A296081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(τ(n)-1, σ(n)-1) where τ is the divisor count and σ is
 *            the sum of divisors
 * Range:     u8 n <= 1330, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 8796 (2026-11-01)
 * Helpers:   gcd, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    /* τ(n) - 1 */
    seq_t tau = (seq_t)num_divisors((uint64_t)n);
    tau = tau - (seq_t)1;

    /* σ(n) - 1 */
    seq_t sigma = sigma_of(n);
    sigma = sigma - (seq_t)1;

    /* gcd of the two values */
    return gcd(tau, sigma);
}
