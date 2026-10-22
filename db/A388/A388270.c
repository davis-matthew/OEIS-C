/*
 * A388270  a(n) = d(n) * gcd(n, sigma(n)), where d is the number of divisors
 *          function (A000005), and sigma is the sum of divisors.
 * https://oeis.org/A388270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) * gcd(n, sigma(n))
 * Range:     u8 n <= 83, u16 n <= 4319, u32 n <= 9878, u64 n <= 9878,
 *            u128 n <= 9878, big any n
 * Helpers:   divisor_count_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* compute sigma(n) (sum of divisors) */
    seq_t sigma = sigma_of(n);

    /* compute d(n) (number of divisors) */
    seq_t d = (seq_t)divisor_count_u64(n);

    /* compute gcd(n, sigma(n)) */
    seq_t g = gcd((seq_t)n, sigma);

    /* return the product */
    return d * g;
}
