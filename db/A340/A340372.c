/*
 * A340372  a(n) = 1 if n has no more than one odd prime factor to an odd
 *          power in its prime factorization, 0 otherwise.
 * https://oeis.org/A340372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has no more than one odd prime factor to an odd
 *            power, 0 otherwise
 * Range:     u8 n <= 9409, u16 n <= 9409, u32 n <= 9409, u64 n <= 9409,
 *            u128 n <= 9409, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined before offset, safe guard */
    uint64_t x = (uint64_t)n;
    int oddOdd = 0;               /* number of odd prime factors with odd exponent */

    /* remove all factors of 2 */
    while ((x & 1ULL) == 0) x >>= 1;

    /* trial division for odd primes */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            if (e & 1) ++oddOdd;
            if (oddOdd > 1) return 0; /* early exit */
        }
    }

    /* any remaining factor is an odd prime with exponent 1 */
    if (x > 1) {
        /* x is odd because all 2's removed earlier */
        ++oddOdd;
    }

    return (oddOdd <= 1) ? (seq_t)1 : (seq_t)0;
}
