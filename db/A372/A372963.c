/*
 * A372963  a(n) = Sum_{1 <= x_1, x_2, x_3, x_4 <= n} ( n/gcd(x_1, x_2, x_3,
 *          x_4, n) )^2.
 * https://oeis.org/A372963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * (n/d)^2 * sigma_6(d) Multiplicative
 *            evaluation: a(p^e) = (p^(6e+6) - p^(6e+2) + p^2 - 1) / (p^6 - 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1626,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 46 terms through n = 8559 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For e = 1 the above simplifies to a(p) = p^6 - p^2 + 1, avoiding
   the need for p^12 and keeping all intermediate values within
   128‑bit arithmetic. */

#include <stdint.h>
#include <stdbool.h>

/* 128‑bit power: computes base^exp for unsigned 64‑bit integers   */
static inline unsigned __int128 pow128_uint64(uint64_t base, uint64_t exp)
{
    unsigned __int128 result = 1;
    unsigned __int128 b = base;
    while (exp)
    {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/*  Compute a(p^e) where p is a prime and e >= 1   */
static seq_t term_prime_power(idx_t p, int e)
{
    if (e == 1)                          /* simplify to avoid p^12       */
    {
        unsigned __int128 p2 = pow128_uint64(p, 2);
        unsigned __int128 p6 = pow128_uint64(p, 6);
        return (seq_t)(p6 - p2 + 1);     /* p^6 - p^2 + 1                */
    }

    unsigned __int128 p2   = pow128_uint64(p, 2);
    unsigned __int128 p6   = pow128_uint64(p, 6);
    unsigned __int128 denom = p6 - 1;          /* p^6 - 1                     */
    unsigned __int128 exp6e = 6ULL * (uint64_t)e; /* 6*e  (<= 78 for n <= 10000) */
    unsigned __int128 p6e   = pow128_uint64(p, exp6e);   /* p^(6e)   */
    unsigned __int128 p6e2  = p6e * p2;              /* p^(6e+2) */
    unsigned __int128 p6e6  = p6e * p6;              /* p^(6e+6) */
    unsigned __int128 numer = p6e6 - p6e2 + p2 - 1;   /* numerator   */
    unsigned __int128 term128 = numer / denom;      /* integer division, exact   */
    return (seq_t)term128;
}

/* Main function for the OEIS sequence A372963 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1          */
    seq_t result = 1;
    idx_t m = n;                         /* factorise n                   */

    /* trial division – works for n up to at least 10000       */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            result *= term_prime_power(p, e);
        }
    }

    /* any remaining prime factor > sqrt(original n) has e==1   */
    if (m > 1)
        result *= term_prime_power(m, 1);

    return result;
}
