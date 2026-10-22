/*
 * A346512  a(n) = bitwise XOR of decimal digits of primes.
 * https://oeis.org/A346512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = bitwise XOR of decimal digits of the nth prime
 * Range:     u8 n <= 54, u16 n <= 7309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t prime = get_prime(n);          /* nth prime */
    seq_t x = prime;
    seq_t res = 0;
    while (x > 0) {
        res ^= x % 10; /* XOR next decimal digit */
        x /= 10;
    }
    return res;
}
