/*
 * A349660  Numbers which are the sum of a prime and the square of the next
 *          prime.
 * https://oeis.org/A349660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + prime(n+1)^2
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 6541, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* compute the nth prime and the next prime */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);
    return (seq_t)p1 + (seq_t)p2 * (seq_t)p2;
}
