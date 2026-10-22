/*
 * A348049  a(n) = A003959(n) / gcd(sigma(n), A003959(n)), where A003959 is
 *          multiplicative with a(p^e) = (p+1)^e and sigma is the sum of
 *          divisors function.
 * https://oeis.org/A348049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003959(n) / gcd(sigma(n), A003959(n))
 * Range:     u8 n <= 16, u16 n <= 1384, u32 n <= 9814, u64 n <= 9814,
 *            u128 n <= 9814, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* small helper: compute base^exp for unsigned exp (exponent fits in
   an unsigned integer).                                                */
static seq_t pow_seq_uint(seq_t base, unsigned exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* small helper: Euclidean gcd for seq_t values.                       */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Compute the n‑th term of OEIS sequence A348049.                     */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset starts at 1; value for 0 not defined */
        return (seq_t)0;

    uint64_t m = (uint64_t)n;          /* working copy for factorisation */
    seq_t a003959 = (seq_t)1;          /* product of (p+1)^e                */
    seq_t sigma   = (seq_t)1;          /* product of sigma factors          */

    while (m > 1)
    {
        /* trial division up to sqrt(m) for the prime factor p */
        uint64_t p;
        for (p = 2; p * p <= m; ++p)
            if (m % p == 0)
                break;
        if (p * p > m)               /* m itself is prime */
            p = m;

        /* count exponent e of prime p in n */
        unsigned e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        /* (p+1)^e contributes to A003959(n) */
        a003959 *= pow_seq_uint((seq_t)(p + 1), e);

        /* sigma factor: (p^(e+1)-1)/(p-1) */
        seq_t p_power   = pow_seq_uint((seq_t)p, e + 1);
        seq_t numerator = p_power - (seq_t)1;
        seq_t denom     = (seq_t)(p - 1);
        sigma *= numerator / denom;
    }

    /* result = A003959(n) / gcd( sigma(n), A003959(n) ) */
    seq_t g = gcd_seq(a003959, sigma);
    return a003959 / g;
}
