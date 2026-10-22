/*
 * A386406  Length of the preperiodic part of the decimal expansion of 1/n,
 *          including any leading zeros from the period.
 * https://oeis.org/A386406
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = p + floor(log10(n/r)) where p = max{e2(n), e5(n)} and r =
 *            10^p mod n, r≠0. If r = 0 then a(n) = p
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: multiply two numbers modulo mod without overflow. */
static inline idx_t mul_mod(idx_t a, idx_t b, idx_t mod)
{
    idx_t res = 0;
    a %= mod;
    while (b)
    {
        if (b & 1)
        {
            res += a;
            if (res >= mod) res -= mod;
        }
        /* double a modulo mod */
        if (a >= mod - a)
            a = a - (mod - a);      /* a = 2a - mod */
        else
            a = a + a;
        b >>= 1;
    }
    return res;
}

/* Helper: compute 10^p modulo mod. */
static inline idx_t pow10_mod(idx_t p, idx_t mod)
{
    idx_t r = 1;
    for (idx_t i = 0; i < p; ++i)
        r = mul_mod(r, 10, mod);
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute the exponent of 2 and 5 in n. */
    idx_t temp = n, e2 = 0, e5 = 0;
    while ((temp & 1) == 0) { ++e2; temp >>= 1; }
    while (temp % 5 == 0) { ++e5; temp /= 5; }
    idx_t p = (e2 > e5) ? e2 : e5;

    /* Compute r = 10^p mod n. */
    idx_t r = pow10_mod(p, n);

    if (r == 0)
        return (seq_t)p;

    /* Compute floor(log10(n/r)) as number of digits of n/r minus 1. */
    idx_t t = n / r;
    idx_t digits = 0;
    idx_t tmp = t;
    do
    {
        ++digits;
        tmp /= 10;
    } while (tmp);

    idx_t extra = digits - 1;           /* floor(log10(t)) */
    return (seq_t)(p + extra);
}
