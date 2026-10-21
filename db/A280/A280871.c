/*
 * A280871  Denominator of the mediant of prime(n) / prime(n+1) and prime(n+2)
 *          / prime(n+3).
 * https://oeis.org/A280871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (prime(n+1)+prime(n+3)) / gcd(prime(n)+prime(n+2),
 *            prime(n+1)+prime(n+3))
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;
    }

    /* Retrieve the four consecutive primes needed */
    uint64_t p0 = (uint64_t)get_prime(n);
    uint64_t p1 = (uint64_t)get_prime(n + 1);
    uint64_t p2 = (uint64_t)get_prime(n + 2);
    uint64_t p3 = (uint64_t)get_prime(n + 3);

    /* Numerator and denominator of the mediant */
    uint64_t num = p0 + p2;
    uint64_t den = p1 + p3;

    /* Reduce the fraction: denominator / gcd(numerator, denominator) */
    seq_t g   = gcd((seq_t)num, (seq_t)den);
    uint64_t g64 = (uint64_t)g;
    uint64_t res64 = den / g64;

    return (seq_t)res64;
}
