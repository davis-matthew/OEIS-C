/*
 * A275697  Primes p such that gcd(p-1, q-1) = q - p, where q is the next
 *          prime after p.
 * https://oeis.org/A275697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that gcd(p-1, q-1) = q-p, where q is * the
 *            next prime after p
 * Range:     u8 n <= 33, u16 n <= 2648, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility prototypes required by the build. */
#include "../utils/gcd.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; values for n < 1 are irrelevant. */
    if (n < 1)
        return 0;

    /* Count of primes satisfying the condition found so far. */
    idx_t count = 0;

    /* Start with the first prime. */
    uint64_t p = 2;

    while (1) {
        /* Next prime after p. */
        uint64_t q = next_prime(p);

        /* Test the defining condition. */
        seq_t g = gcd((seq_t)(p - 1), (seq_t)(q - 1));
        if (g == (seq_t)(q - p)) {
            ++count;
            if (count == n)
                return (seq_t)p;           /* nth qualifying prime found. */
        }

        /* Advance to the next prime. */
        p = q;
    }
}
