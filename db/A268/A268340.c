/*
 * A268340  Characteristic function of the prime powers p^k, k >= 2.
 * https://oeis.org/A268340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is a prime power p^k with k ≥ 2, otherwise 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9911 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 iff n = p^k with k >= 2: divide out the smallest prime factor p
   of n and check that what is left is a power of p, not 1 at once. (The
   original rebuilt a sieve for every new n and freed it as a seq_t array.) */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n, p = 2;
    if (m < 4) return (seq_t)0;
    while (p * p <= m && m % p) ++p;
    if (m % p) return (seq_t)0;              /* n is prime */
    m /= p;
    while (m % p == 0) m /= p;
    return (seq_t)(m == 1);
}
