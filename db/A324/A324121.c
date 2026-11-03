/*
 * A324121  a(n) = gcd(n*d(n), sigma(n)), where d(n) = number of divisors of n
 *          (A000005) and sigma(n) = sum of divisors of n (A000203).
 * https://oeis.org/A324121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n*d(n), sigma(n))
 * Range:     u8 n <= 39, u16 n <= 5081, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 121 terms through n = 9603 (2026-11-01)
 * Helpers:   gcd, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   d(n) = number of divisors of n (A000005)               */
/*   sigma(n) = sum of divisors of n (A000203)              */

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
    /* n is at least 1 (OEIS offset 1) */
    seq_t nd = (seq_t)n * (seq_t)num_divisors(n);
    seq_t sig = sigma_of(n);
    return gcd(nd, sig);
}
