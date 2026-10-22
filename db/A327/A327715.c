/*
 * A327715  a(0) = 0; for n >= 1, a(n) = 1 + a(n-A009191(n)).
 * https://oeis.org/A327715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of steps to reach zero starting from k = n, where in each step
   we replace k by k - gcd(k, d(k)) and d(k) is the number of divisors of k.
   a(0) = 0; for n >= 1, a(n) = 1 + a(n - gcd(n, d(n))). */
#include <stdbool.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t a, seq_t b) */

static inline seq_t divisor_count(idx_t n)
{
    seq_t cnt = 1u;
    idx_t x   = n;
    for (idx_t p = 2; p <= x / p; ++p)
    {
        if (x % p == 0)
        {
            idx_t e = 0;
            while (x % p == 0)
            {
                x /= p;
                ++e;
            }
            cnt *= (seq_t)(e + 1u);
        }
    }
    if (x > 1)   /* remaining prime factor */
        cnt *= 2u;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0u;

    seq_t steps = 0u;
    while (n > 0u)
    {
        seq_t d = divisor_count(n);
        seq_t g = gcd((seq_t)n, d);
        n -= (idx_t)g;
        ++steps;
    }
    return steps;
}
