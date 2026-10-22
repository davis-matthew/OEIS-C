/*
 * A349350  Dirichlet inverse of A057521, the powerful part of n.
 * https://oeis.org/A349350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Dirichlet inverse of A057521 (the powerful part of n)
 * Range:     i8 n <= 168, i16 n <= 9518, i32 n <= 9518, i64 n <= 9518,
 *            i128 n <= 9518, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, and for n>1 */
/*    a(n) = - Σ_{d|n,d<n} A057521(d) * a(n/d). */
/* For a prime power p^e, */
/*    A057521(p^e) = 1 if e=1 else p^e. */
/* This implementation computes a(n) multiplicatively. */

#include <stdint.h>
#include <stdbool.h>

/* Compute p^e for seq_t base, integer exponent e. */
static seq_t seq_pow_uint(seq_t base, unsigned e)
{
    seq_t res = 1;
    while (e)
    {
        if (e & 1) res *= base;
        base *= base;
        e >>= 1;
    }
    return res;
}

/* Compute a(p^e) using the recurrence. e >= 1. */
static seq_t a_prime_power(uint64_t p, unsigned e)
{
    static seq_t a[65];           /* support e up to 64 (uint64 power) */
    a[0] = 1;
    for (unsigned i = 1; i <= e; ++i)
    {
        seq_t sum = 0;
        for (unsigned k = 0; k < i; ++k)
        {
            unsigned exp = i - k;                      /* exponent of p in f(p^exp) */
            seq_t f = (exp < 2) ? 1 : seq_pow_uint((seq_t)p, exp);
            sum += a[k] * f;                           /* a(p^k) * f(p^exp) */
        }
        a[i] = -sum;
    }
    return a[e];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */
    if (n == 1) return 1;          /* a(1) = 1 */

    seq_t result = 1;
    uint64_t x = (uint64_t)n;

    /* Factor using trial division. */
    for (uint64_t p = 2; p <= x / p; ++p)
    {
        if (x % p == 0)
        {
            unsigned e = 0;
            while (x % p == 0)
            {
                x /= p;
                ++e;
            }
            result *= a_prime_power(p, e);
        }
    }
    if (x > 1)                     /* remaining prime factor */
        result *= a_prime_power(x, 1);

    return result;
}
