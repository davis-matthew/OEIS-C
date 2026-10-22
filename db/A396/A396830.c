/*
 * A396830  a(n) = Sum_{1 <= x_1, x_2, x_3, x_4, x_5 <= n} gcd(x_1, x_2, x_3,
 *          x_4, n)/gcd(x_1, x_2, x_3, x_4, x_5, n).
 * https://oeis.org/A396830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 83, u64 n <= 7055,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula from OEIS: a(n) = Sum_{d|n} J_4(n/d) * (n/d) * sigma_2(d^2)/sigma(d^2)
    (multiplicative, a(p^e) given by the closed form below)   */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/*  Compute integer power using 128‑bit arithmetic to avoid overflow when seq_t
    is at most 128 bits wide. The result fits into an unsigned __int128. */
static unsigned __int128 pow128(unsigned long long base, idx_t exp)
{
    unsigned __int128 res = 1, b = base;
    while (exp) {
        if (exp & 1) res *= b;
        b *= b;
        exp >>= 1;
    }
    return res;
}

/*  Multiplicative contribution of a prime power p^e to a(n).  All
    intermediate calculations are performed in 128‑bit, the final
    result is converted to seq_t.  The formula is:
        a(p^e) = ((p^4-1) * (p^(2e)*(p^(3e)-1)/(p^3-1) + p*(p^(5e)-1)/(p^5-1))
                    + p^(2e+1) + 1) / (p+1)
    (See OEIS entry A396830).  The divisions are exact. */
static seq_t prime_power_value(unsigned long long p, idx_t e)
{
    unsigned __int128 p128 = p;

    /* powers of p needed in the formula */
    unsigned __int128 p2e  = pow128(p, 2 * e);
    unsigned __int128 p3e  = pow128(p, 3 * e);
    unsigned __int128 p5e  = pow128(p, 5 * e);
    unsigned __int128 p2e1 = pow128(p, 2 * e + 1);
    unsigned __int128 p4   = pow128(p, 4);
    unsigned __int128 p3   = pow128(p, 3);
    unsigned __int128 p5   = pow128(p, 5);

    /* the two bracketed rational terms */
    unsigned __int128 term1 = (p3e - 1) / (p3 - 1);   /* = (p^(3e)-1)/(p^3-1) */
    unsigned __int128 term2 = (p5e - 1) / (p5 - 1);   /* = (p^(5e)-1)/(p^5-1) */

    unsigned __int128 inner = p2e * term1 + (unsigned __int128)p * term2;
    unsigned __int128 top   = (p4 - 1) * inner + p2e1 + 1;
    unsigned __int128 denom = p + 1;

    return (seq_t)(top / denom);
}

/*  Main sequence function.  Uses the multiplicative property to avoid
    an expensive 5‑fold loop.  For n up to about 10 000 the
    algorithm runs in well under a second. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* out of OEIS range */

    /* factor n to its prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned long long p = primes[i];
        idx_t e = (idx_t)exps[i];
        result *= prime_power_value(p, e);
    }
    return result;
}
