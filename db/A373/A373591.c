/*
 * A373591  Number of primes congruent to 1 modulo 3 dividing n (with
 *          multiplicity).
 * https://oeis.org/A373591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors of n that are ≡ 1 (mod 3),
 *            counted with multiplicity
 * Range:     u8 n <= 9787, u16 n <= 9787, u32 n <= 9787, u64 n <= 9787,
 *            u128 n <= 9787, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0, a(0) is irrelevant */
        return 0;

    uint64_t x = (uint64_t)n;
    seq_t count = 0;

    while (x > 1) {
        uint64_t p = _smallest_prime_factor(x);
        if (p == 0 || p == x)   /* x itself is prime */
            p = x;

        while (x % p == 0) {
            if (p % 3ULL == 1ULL)
                count += 1;
            x /= p;
        }
    }

    return count;
}
