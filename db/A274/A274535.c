/*
 * A274535  a(n) = 5*sigma(n).
 * https://oeis.org/A274535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*sigma(n)
 * Range:     u8 n <= 23, u16 n <= 3779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* sigma_of returns the sum of all positive divisors of n */
    seq_t s = sigma_of(n);
    return 5 * s;
}
