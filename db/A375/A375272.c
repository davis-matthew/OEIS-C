/*
 * A375272  The number of factors of n of the form p^Fibonacci(k), where p is
 *          a prime and k >= 2, when the factorization is uniquely done using
 *          the dual Zeckendorf representation of the exponents in the prime
 *          factorization of n.
 * https://oeis.org/A375272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum_{p^e || n} A112310(e) (A112310(e) is the maximum
 *            number of distinct Fibonacci numbers summing to e, i.e. the
 *            number of summands in the unique dual‐Zeckendorf representation
 *            of e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t N)
{
    if (N <= 1) return 0;                    /* a(1) = 0 */

    uint64_t n = (uint64_t)N;
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;

    /* factorize n */
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 0;

    /* pre‑allocate a small static array for DP (exponent < 64 for 64‑bit n) */
    int dp[128];

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        if (e == 0) continue;               /* no contribution (should not happen) */

        /* build Fibonacci numbers <= e (starting with Fib(2)=1, Fib(3)=2) */
        int fibs[64];
        size_t fcnt = 0;

        uint64_t f1 = 1, f2 = 2;
        if (e >= 1) fibs[fcnt++] = (int)f1;   /* 1 */

        while (f2 <= e)
        {
            fibs[fcnt++] = (int)f2;
            uint64_t tmp = f1 + f2;
            f1 = f2;
            f2 = tmp;
        }

        /* DP for maximum number of distinct Fibonacci summands */
        int maxE = (int)e;
        for (int s = 0; s <= maxE; ++s) dp[s] = -1000;
        dp[0] = 0;

        for (size_t j = 0; j < fcnt; ++j)
        {
            int f = fibs[j];
            for (int s = maxE; s >= f; --s)
            {
                if (dp[s - f] >= 0)
                {
                    int cand = dp[s - f] + 1;
                    if (cand > dp[s]) dp[s] = cand;
                }
            }
        }

        result += (seq_t)dp[maxE];
    }

    return result;
}
