/*
 * A377319  a(n) is the smallest positive integer k such that n + k and n - k
 *          have the same number of divisors.
 * https://oeis.org/A377319
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive k such that d(n+k) = d(n-k) where d(m)
 *            is the number of divisors of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Simple divisor count for uint64_t numbers.
   Works well for the small n and k encountered (n <= 10'000). */
static inline uint64_t divisor_count(uint64_t x)
{
    if (x <= 1) return 1;
    uint64_t cnt = 1;
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            uint64_t exp = 0;
            while (x % p == 0)
            {
                x /= p;
                ++exp;
            }
            cnt *= (exp + 1);
        }
    }
    if (x > 1) cnt *= 2;   /* remaining prime factor */
    return cnt;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 4, but we handle all n >= 1
       gracefully by searching k until n-k >= 1. */
    for (idx_t k = 1;; ++k)
    {
        if (k >= n) break;                 /* n-k would be 0 or negative */
        seq_t d1 = divisor_count((uint64_t)n + k);
        seq_t d2 = divisor_count((uint64_t)n - k);
        if (d1 == d2) return k;
    }
    return 0;   /* never reached for valid n */
}
