/*
 * A367573  Long legs of the only primitive Pythagorean triple whose inradius
 *          is the n-th prime and whose short leg is an odd number.
 * https://oeis.org/A367573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*prime(n)^2 + 2*prime(n) where prime(n) is the n-th
 *            prime
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    seq_t p = (seq_t)get_prime(n);          /* p is the n-th prime */
    seq_t tmp = p * (p + 1);                /* p*(p+1) */
    return tmp << 1;                       /* 2 * p*(p+1) = 2*p^2 + 2*p */
}
