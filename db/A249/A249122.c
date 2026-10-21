/*
 * A249122  a(n) = floor(n / lpf(n^2 + 1)) where lpf(n^2 + 1) is the smallest
 *          prime divisor of n^2 + 1.
 * https://oeis.org/A249122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / lpf(n^2 + 1)) where lpf(m) is the smallest *
 *            prime divisor of m. This implementation computes lpf by trial
 *            division, * which is fast enough for the typical test range (n ≤
 *            10000)
 * Range:     u8 n <= 15, u16 n <= 8270, u32 n <= 8270, u64 n <= 8270,
 *            u128 n <= 8270, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Simple trial division to find the smallest prime factor of a 64‑bit
 * integer.  This is adequate for m≤10^8+1 (the case n≤10 000).  */
static uint64_t smallest_prime_factor(uint64_t m)
{
    if (m < 2) return m;             /* 0 or 1 has no prime factor */
    if (m % 2 == 0) return 2;
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) return d;
    }
    return m;          /* m is prime */
}

seq_t a(idx_t n)
{
    /* Compute n^2 + 1 without overflow using 128‑bit arithmetic.  */
    unsigned __int128 sq = (unsigned __int128)n * n + 1;
    /* The sequence definition only makes sense when n^2+1 fits in idx_t
       (uint64_t).  For the test range this is always true.  */
    uint64_t val = (uint64_t)sq;
    uint64_t lpf = smallest_prime_factor(val);
    return (seq_t)n / (seq_t)lpf;
}
