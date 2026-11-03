/*
 * A373591  Number of primes congruent to 1 modulo 3 dividing n (with
 *          multiplicity).
 * https://oeis.org/A373591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors of n that are ≡ 1 (mod 3),
 *            counted with multiplicity
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9787 (2026-11-01)
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0, a(0) is irrelevant */
        return 0;

    uint64_t x = (uint64_t)n;
    seq_t count = 0;

    while (x > 1) {
        uint64_t p = smallest_prime_factor(x);
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
