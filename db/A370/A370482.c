/*
 * A370482  Characteristic function of primes plus characteristic function of
 *          even numbers.
 * https://oeis.org/A370482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = is_prime(n) + is_even(n)
 * Range:     u8 n <= 8534, u16 n <= 8534, u32 n <= 8534, u64 n <= 8534,
 *            u128 n <= 8534, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t val = 0;
    if ((n & 1ULL) == 0)             /* even n */
        val += (seq_t)1;
    if (is_prime(n))                 /* primality test */
        val += (seq_t)1;
    return val;
}
