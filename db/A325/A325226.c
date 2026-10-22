/*
 * A325226  Number of prime factors of n that are less than the largest,
 *          counted with multiplicity.
 * https://oeis.org/A325226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(n) - A071178(n) Number of prime factors of n that
 *            are less than the largest, counted with multiplicity
 * Range:     u8 n <= 9958, u16 n <= 9958, u32 n <= 9958, u64 n <= 9958,
 *            u128 n <= 9958, big any n
 * Helpers:   bigomega, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* a(1) is defined to be 0 */
    if (n <= 1)
        return 0;

    /* Largest prime factor of n */
    uint64_t lpf = largest_prime_factor(n);

    /* Count how many times this largest prime occurs in n */
    seq_t e = 0;
    uint64_t tmp = n;
    while (tmp % lpf == 0) {
        tmp /= lpf;
        e++;
    }

    /* Total number of prime factors (with multiplicity) */
    seq_t total = bigomega(n);

    /* Prime factors less than the largest, counted with multiplicity */
    return total - e;
}
