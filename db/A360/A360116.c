/*
 * A360116  a(n) = 1 if there are no prime factors p of n for which the 2-adic
 *          valuation of p-1 is less than that of n-1, otherwise 0.
 * https://oeis.org/A360116
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff for every prime factor p of n, the 2‑adic valuation
 *            of p‑1 is less than or equal to that of n‑1; otherwise a(n) = 0
 * Range:     u8 n <= 9846, u16 n <= 9846, u32 n <= 9846, u64 n <= 9846,
 *            u128 n <= 9846, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utilities for 2‑adic valuation using GCC/Clang builtin.  The
   builtin is fast and works for 64‑bit unsigned integers.  We
   never pass 0 to it. */
static inline unsigned v2_uint64(uint64_t x)
{
    return __builtin_ctzll(x);
}

seq_t a(idx_t n)
{
    /* Outside the offset (n < 2) – behaviour not specified. */
    if (n < 2)
        return 0;

    /* Convert to 64‑bit unsigned for bitwise operations. */
    uint64_t nn = (uint64_t)n;

    /* 2‑adic valuation of n‑1. */
    unsigned v2_n = v2_uint64(nn - 1);

    /* Factorization of n using simple trial division.
       Only distinct prime factors are needed. */
    uint64_t m = nn;

    /* Handle factor 2 separately. */
    if ((m & 1ULL) == 0) {
        unsigned v2_p1 = v2_uint64(2ULL - 1); /* v2(1) = 0 */
        if (v2_p1 > v2_n)
            return 0;
        while ((m & 1ULL) == 0)
            m >>= 1;
    }

    /* Odd prime factors. */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            unsigned v2_p1 = v2_uint64(p - 1);
            if (v2_p1 > v2_n)
                return 0;
            while (m % p == 0)
                m /= p;
        }
    }

    /* If m > 1, it is a remaining prime factor. */
    if (m > 1) {
        unsigned v2_p1 = v2_uint64(m - 1);
        if (v2_p1 > v2_n)
            return 0;
    }

    return (seq_t)1;
}
