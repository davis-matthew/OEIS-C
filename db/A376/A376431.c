/*
 * A376431  a(n) is the least odd prime factor of prime(n)^prime(n)-1.
 * https://oeis.org/A376431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least odd prime divisor of prime(n)^prime(n)-1
 * Range:     u8 n <= 6, u16 n <= 6542, u32 n <= 6542, u64 n <= 6542,
 *            u128 n <= 6542, big any n
 * Helpers:   get_prime, next_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/powmod64.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1-indexed) */
    uint32_t p = get_prime(n);

    /* Iterate odd primes until we find one dividing p^p - 1 */
    uint64_t q = 3ULL;  /* smallest odd prime */
    while (1) {
        /* p^p mod q == 1  <=>  q | (p^p - 1) */
        if (powmod64((uint64_t)p, (uint64_t)p, q) == 1ULL) {
            return (seq_t)q;
        }
        q = next_prime(q);  /* advance to the next odd prime */
    }
}
