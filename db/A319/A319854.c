/*
 * A319854  Number of ways to write n as the sum of 4 positive integers a, b,
 *          c, d such that d < b and a/b - c/d = (a - c)/(b + d).
 * https://oeis.org/A319854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   Count solutions of a*d^2 = b^2*c with a+b+c+d = n and d < b
 * Range:     u8 n <= 1051, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates coprime pairs (b',d'), chooses k and g
   from the derived formula.  It runs in roughly O(n^(3/2)) time. */

#include <stdbool.h>
#include <stdint.h>

static unsigned long long gcd_u64(unsigned long long a, unsigned long long b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static unsigned long long isqrt_u64(unsigned long long n)
{
    unsigned long long lo = 0, hi = n;
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    unsigned long long N = (unsigned long long)n;
    seq_t total = 0;

    unsigned long long max_b = isqrt_u64(N);

    for (unsigned long long bp = 2; bp <= max_b; ++bp) {
        for (unsigned long long dp = 1; dp < bp; ++dp) {
            if (gcd_u64(bp, dp) != 1)
                continue;

            unsigned long long s2 = bp * bp + dp * dp;
            unsigned long long sum1 = bp + dp;

            if (s2 > N - sum1)
                continue;                 /* no possible k with g>=1 */

            unsigned long long maxk = (N - sum1) / s2;
            for (unsigned long long k = 1; k <= maxk; ++k) {
                unsigned long long t = N - k * s2;
                if (t % sum1 == 0) {
                    /* g = t / sum1 is guaranteed >= 1 by construction of maxk */
                    total += 1;
                }
            }
        }
    }

    return total;
}
