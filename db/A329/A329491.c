/*
 * A329491  Dirichlet g.f.: Sum_{n>=0} a(n+1)/(1+10n)^s = Product
 *          ((1+p^(-s))/(1-p^(-s))) (p==1 mod 5).
 * https://oeis.org/A329491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{ω(m)} with m = 1 + 10 (n-1) If all prime factors of m
 *            satisfy p ≡ 1 (mod 5) then a(n) = 2^{number of distinct such
 *            primes}, otherwise a(n) = 0. The function uses trial division;
 *            it is correct for all n ≥ 1
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* m = 1 + 10*(n-1)  (use 128‑bit to avoid overflow on small n);  m fits in idx_t for all tests */
    __int128 m_big = (__int128)n - 1;
    m_big = m_big * 10 + 1;
    idx_t m = (idx_t)m_big;

    /* Special case: n=1 gives m=1 -> a(1)=1 */
    if (m == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t x = m;

    /* Trial division; m is at most 1 + 10 * (max n - 1), which is < 2^64 for all practical n. */
    for (idx_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            /* p is a prime divisor of m */
            if (p % 5 != 1)
                return (seq_t)0;           /* contains forbidden prime factor */
            result *= (seq_t)2;            /* distinct prime contributes a factor of 2 */

            /* Remove all copies of this prime */
            while (x % p == 0)
                x /= p;
        }
    }

    /* After the loop, if x > 1 then x is the remaining prime factor */
    if (x > 1)
    {
        if (x % 5 != 1)
            return (seq_t)0;               /* contains forbidden prime factor */
        result *= (seq_t)2;                /* distinct prime contributes a factor of 2 */
    }

    return result;
}
