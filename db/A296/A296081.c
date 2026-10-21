/*
 * A296081  a(n) = gcd(tau(n)-1, sigma(n)-1), where tau = A000005 and sigma =
 *          A000203.
 * https://oeis.org/A296081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(τ(n)-1, σ(n)-1) where τ is the divisor count and σ is
 *            the sum of divisors
 * Range:     u8 n <= 1330, u16 n <= 8796, u32 n <= 8796, u64 n <= 8796,
 *            u128 n <= 8796, big any n
 * Helpers:   divisor_count_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    /* τ(n) - 1 */
    seq_t tau = (seq_t)divisor_count_u64((uint64_t)n);
    tau = tau - (seq_t)1;

    /* σ(n) - 1 */
    seq_t sigma = sigma_of(n);
    sigma = sigma - (seq_t)1;

    /* gcd of the two values */
    return gcd(tau, sigma);
}
