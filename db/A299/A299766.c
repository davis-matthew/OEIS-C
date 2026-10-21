/*
 * A299766  Greatest odd noncomposite divisor of n.
 * https://oeis.org/A299766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest odd noncomposite divisor of n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   _largest_odd_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 1 if n is a power of 2, otherwise
 * the largest odd prime factor of n. */

#include <stdint.h>
#include "../utils/_largest_odd_prime_factor.h"

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
