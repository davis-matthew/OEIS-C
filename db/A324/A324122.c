/*
 * A324122  a(n) = sigma(n) - gcd(n*d(n), sigma(n)), where d(n) = number of
 *          divisors of n (A000005) and sigma(n) = sum of divisors of n
 *          (A000203).
 * https://oeis.org/A324122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - gcd(n * d(n), sigma(n))
 * Range:     u8 n <= 39, u16 n <= 5333, u32 n <= 9812, u64 n <= 9812,
 *            u128 n <= 9812, big any n
 * Helpers:   divisor_count_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute sigma(n) */
    seq_t sigma = sigma_of(n);

    /* compute d(n) = number of divisors */
    seq_t d = (seq_t)divisor_count_u64(n);

    /* compute n * d(n) */
    seq_t prod = (seq_t)n * d;

    /* gcd of prod and sigma */
    seq_t g = gcd(prod, sigma);

    /* final result */
    return sigma - g;
}
