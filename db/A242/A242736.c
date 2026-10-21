/*
 * A242736  Number of solutions of a^2 + b^2 congruent to -1 modulo the n-th
 *          prime.
 * https://oeis.org/A242736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 1; for n > 1, a(n) = ceil(p(n)/8) where p(n) is
 *            the n-th prime
 * Range:     u8 n <= 54, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    /* get_prime returns the n-th prime as uint32_t */
    uint32_t p = get_prime(n);
    return ((seq_t)p + 7) / 8;
}
