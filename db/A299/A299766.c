/*
 * A299766  Greatest odd noncomposite divisor of n.
 * https://oeis.org/A299766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest odd noncomposite divisor of n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 1 if n is a power of 2, otherwise
 * the largest odd prime factor of n. */

#include <stdint.h>

/* Helper: compute the largest odd prime factor of an odd number m > 1.
 * m is odd, so we test only odd divisors. */
static seq_t _largest_odd_prime_factor(idx_t m)
{
    seq_t largest = 1;
    idx_t temp = m;

    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            largest = d;                /* d is a prime factor (odd) */
            while (temp % d == 0) {
                temp /= d;              /* strip all copies of d */
            }
        }
    }

    /* If what's left is > 1, it's a prime factor larger than sqrt(m). */
    if (temp > 1 && (seq_t)temp > largest) {
        largest = (seq_t)temp;
    }
    return largest;
}

seq_t a(idx_t n)
{
    /* Remove all factors of 2; the remaining odd part is m. */
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
    }

    /* If m <= 1, n was a power of two (or 1), answer is 1. */
    if (m <= 1) {
        return (seq_t)1;
    }

    /* Otherwise return the largest odd prime factor of m. */
    return _largest_odd_prime_factor(m);
}
