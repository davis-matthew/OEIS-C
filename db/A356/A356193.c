/*
 * A356193  a(n) is the smallest cubefull number (A036966) that is a multiple
 *          of n.
 * https://oeis.org/A356193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is the smallest cubefull number that is a multiple of n.
 *            For a prime factor p of n with exponent e, the required exponent
 *            in a(n) is max(e,3). Thus a(n) = ∏ p^max(e_p(n),3)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return smallest cubefull multiple of n. */
seq_t a(idx_t n)
{
    /* Work in native 64‑bit unsigned for the factorisation. */
    uint64_t m = (uint64_t)n;
    seq_t    ans = 1;

    /* Handle factor 2 separately. */
    if (m > 1)
    {
        uint64_t exp = 0;
        while ((m & 1ULL) == 0)
        {
            exp++;
            m >>= 1;
        }
        if (exp > 0)
        {
            uint64_t need = (exp >= 3) ? exp : 3;
            seq_t term = 1;
            for (uint64_t i = 0; i < need; ++i)
                term *= (seq_t)2ULL;
            ans *= term;
        }
    }

    /* Handle odd factors. */
    for (uint64_t p = 3; p * p <= m; p += 2)
    {
        if (m % p == 0)
        {
            uint64_t exp = 0;
            while (m % p == 0)
            {
                exp++;
                m /= p;
            }
            uint64_t need = (exp >= 3) ? exp : 3;
            seq_t term = 1;
            for (uint64_t i = 0; i < need; ++i)
                term *= (seq_t)p;
            ans *= term;
        }
    }

    /* If anything remains it is a prime factor with exponent 1. */
    if (m > 1)
    {
        seq_t term = 1;
        for (uint64_t i = 0; i < 3; ++i)
            term *= (seq_t)m;
        ans *= term;
    }

    return ans;
}
