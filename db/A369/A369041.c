/*
 * A369041  LCM-transform of binary Gray code (A003188).
 * https://oeis.org/A369041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014963(G(n)) where G(n) = n XOR (n>>1) (binary Gray
 *            code)
 * Range:     u8 n <= 261, u16 n <= 9394, u32 n <= 9394, u64 n <= 9394,
 *            u128 n <= 9394, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is the prime factor p if G(n) is a power of a single prime p; otherwise 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the n-th binary reflected Gray code. */
    uint64_t g = ((uint64_t)n) ^ (((uint64_t)n) >> 1);

    if (g <= 1)
        return (seq_t)1;                    /* a(1) = 1 and a(0) is not used */

    /* Find the smallest prime factor of g. */
    uint64_t p = 0;
    uint64_t m = g;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            p = d;
            break;
        }
    }

    /* If no factor ≤ sqrt(g) was found, g itself is prime. */
    if (p == 0)
        return (seq_t)g;                    /* g is prime → result is g */

    /* Check whether g is a power of the found prime p. */
    uint64_t rem = g;
    while (rem % p == 0)
        rem /= p;

    if (rem == 1)
        return (seq_t)p;                    /* g = p^k → return the prime p */

    return (seq_t)1;                        /* g has multiple distinct primes → result 1 */
}
