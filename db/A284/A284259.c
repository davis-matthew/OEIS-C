/*
 * A284259  a(n) = number of distinct prime factors of n that are < the square
 *          of smallest prime factor of n, a(1) = 0.
 * https://oeis.org/A284259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of n that are <
 *            (smallest prime factor of n)^2
 * Range:     u8 n <= 9817, u16 n <= 9817, u32 n <= 9817, u64 n <= 9817,
 *            u128 n <= 9817, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the smallest prime factor of an unsigned 64‑bit integer.
   It works for any n ≤ 2^64‑1 using trial division. */
static uint64_t _smallest_prime_factor(uint64_t n)
{
    if (n < 2)
        return n;
    if ((n & 1) == 0)
        return 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if ((n % d) == 0)
            return d;
    }
    return n;               /* n is prime */
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t nn = (uint64_t)n;
    uint64_t spf = _smallest_prime_factor(nn);
    unsigned __int128 spf2 = (unsigned __int128)spf * spf;

    seq_t count = 0;
    uint64_t rem = nn;
    uint64_t last = 0;

    while (rem > 1) {
        uint64_t p = _smallest_prime_factor(rem);
        if (p != last) {
            if ((unsigned __int128)p < spf2)
                ++count;
            last = p;
        }
        /* Remove all copies of p */
        while (rem % p == 0)
            rem /= p;
    }
    return count;
}
