/*
 * A277515  Smallest prime p such that n sqrt(2) < m sqrt(p) < (n+1) sqrt(2)
 *          for some integer m.
 * https://oeis.org/A277515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime p>2 such that floor( sqrt(2*(n+1)^2 / p) )
 *            > floor( sqrt(2*n^2 / p) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt_div_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works for any
   seq_t (uint8_t .. uint64_t, unsigned __int128, or arbitrary-precision integer).
   It uses only integer arithmetic and a simple primality test (trial division). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt_div_uint64.h"

/* very small primality test: works fast for the usually small primes needed. */
static bool is_prime_simple(uint64_t p)
{
    if (p < 2) return false;
    if (p % 2 == 0) return p == 2;
    for (uint64_t d = 3; d * d <= p; d += 2)
        if (p % d == 0)
            return false;
    return true;
}

/* main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset is 1; values for n<1 are irrelevant */

    uint64_t un = (uint64_t)n;
    uint64_t n2 = un * un;

    for (uint64_t p = 3; ; p += 2) {
        if (!is_prime_simple(p))
            continue;

        uint64_t num1 = 2 * n2;
        uint64_t num2 = 2 * (un + 1) * (un + 1);

        uint64_t f1 = floor_sqrt_div_uint64(num1, p);
        uint64_t f2 = floor_sqrt_div_uint64(num2, p);

        if (f2 > f1)
            return (seq_t)p;            /* found the smallest suitable prime */
    }
}
