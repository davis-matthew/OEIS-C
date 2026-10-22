/*
 * A330866  a(n) = Sum_{d|n, d<n} (n/d) * (n-d).
 * https://oeis.org/A330866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * sigma(n) - n * tau(n) where sigma(n) is the sum of
 *            all divisors of n and tau(n) is the number of divisors of n.
 *            This implements the sum over proper divisors: a(n) = Σ_{d|n,
 *            d<n} (n/d) * (n-d)
 * Range:     u8 n <= 11, u16 n <= 167, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper type for temporary 128‑bit multiplication, if available. */
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#else
typedef unsigned long long u128;
#endif

seq_t a(idx_t n)
{
    /* For n == 0 the sequence is undefined; return 0. */
    if (n == 0)
        return (seq_t)0;

    idx_t m = n;
    seq_t sigma = (seq_t)1;   /* product over (1 + p + p² + … + pᵉ) */
    seq_t tau   = (seq_t)1;   /* product over (e + 1)                 */

    /* Trial factorisation up to sqrt(m). */
    for (idx_t p = 2; (u128)p * p <= m; ++p) {
        if (m % p != 0)
            continue;

        int exp = 0;
        seq_t part = (seq_t)1;   /* sum of powers of p */
        seq_t power = (seq_t)1;  /* current p^k */

        while (m % p == 0) {
            m /= p;
            power *= (seq_t)p;
            part += power;
            ++exp;
        }

        sigma *= part;
        tau *= (seq_t)(exp + 1);
    }

    /* If m > 1, then m is a prime factor with exponent 1. */
    if (m > 1) {
        seq_t part = (seq_t)1 + (seq_t)m;  /* 1 + m */
        sigma *= part;
        tau   *= (seq_t)2;
    }

    seq_t nseq = (seq_t)n;
    return nseq * sigma - nseq * tau;
}
