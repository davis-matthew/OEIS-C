/*
 * A392865  a(n) = n mod A117366(n), where A117366 gives the smallest prime
 *          greater than the largest prime dividing n.
 * https://oeis.org/A392865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod A117366(n) where A117366(n) =
 *            next_prime(largest_prime_divisor(n))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a(n) = n mod (smallest prime > largest prime factor of n) */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    /* For n = 1, largest_prime_divisor(1) returns 1 and
       next_prime(1) correctly yields 2, so the generic code works. */
    uint64_t lp = largest_prime_divisor(n);   /* largest prime divisor of n */
    uint64_t p  = next_prime(lp);             /* smallest prime greater than lp */
    seq_t r = (seq_t)(n % p);                 /* n modulo p */
    return r;
}
