/*
 * A388270  a(n) = d(n) * gcd(n, sigma(n)), where d is the number of divisors
 *          function (A000005), and sigma is the sum of divisors.
 * https://oeis.org/A388270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) * gcd(n, sigma(n))
 * Range:     u8 n <= 83, u16 n <= 4319, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 109 terms through n = 9878 (2026-11-01)
 * Helpers:   gcd, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
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
    /* compute sigma(n) (sum of divisors) */
    seq_t sigma = sigma_of(n);

    /* compute d(n) (number of divisors) */
    seq_t d = (seq_t)num_divisors(n);

    /* compute gcd(n, sigma(n)) */
    seq_t g = gcd((seq_t)n, sigma);

    /* return the product */
    return d * g;
}
