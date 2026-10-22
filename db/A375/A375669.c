/*
 * A375669  The maximum exponent in the prime factorization of the largest odd
 *          divisor of n.
 * https://oeis.org/A375669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the maximum exponent of odd primes in the factorization
 *            of * the largest odd divisor of n. For n a power of 2 a(n)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer types */
#include <stdint.h>

/*
 * Return the desired value for a given index n.
 */
seq_t a(idx_t n)
{
    /* Compute the odd part of n: remove all factors of 2. */
    idx_t odd = n;
    while ((odd & 1ULL) == 0ULL) {
        odd >>= 1;
    }

    /* If the odd part is 1 (i.e., n was a power of two), return 0. */
    if (odd <= 1ULL) {
        return 0;
    }

    /* Factor the odd part using simple trial division up to sqrt(x). */
    uint64_t x = odd;           /* x fits in 64 bits for the expected range. */
    seq_t maxexp = 0;           /* Result to be returned. */

    uint64_t p = 3ULL;          /* Start with the first odd prime. */
    while (p * p <= x) {
        if (x % p == 0ULL) {
            seq_t e = 0;        /* Exponent of current prime. */
            while (x % p == 0ULL) {
                x /= p;
                e++;
            }
            if (e > maxexp) {
                maxexp = e;
            }
        }
        /* Move to the next odd prime in a simple way: 3 -> 5 -> 7 -> ... */
        if (p == 3ULL) {
            p = 5ULL;
        } else {
            p += 2ULL;
        }
    }

    /* If anything remains, it is a prime factor with exponent 1. */
    if (x > 1ULL) {
        if (maxexp < (seq_t)1) {
            maxexp = 1;
        }
    }

    return maxexp;
}
