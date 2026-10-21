/*
 * A267113  Bitwise-OR of the exponents of all 4k+1 primes in the prime
 *          factorization of n.
 * https://oeis.org/A267113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bitwise OR of the exponents of all 4k+1 primes in the
 *            prime factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    if (m <= 1) return (seq_t)0;

    seq_t res = 0;

    /* remove all factors 2 – they are not 1 (mod 4) */
    while ((m & 1ULL) == 0)
        m >>= 1;

    /* iterate over odd potential prime divisors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            seq_t e = 0;
            while (m % p == 0) {
                e++;
                m /= p;
            }
            if (p % 4ULL == 1ULL) {
                res |= e;
            }
        }
    }

    /* if a prime factor > 1 remains */
    if (m > 1 && m % 4ULL == 1ULL) {
        res |= 1;
    }

    return res;
}
