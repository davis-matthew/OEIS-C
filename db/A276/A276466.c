/*
 * A276466  a(n) = numerator of Sum_{d|n} 0.d.
 * https://oeis.org/A276466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numerator of ∑_{d|n} 0.d (decimal fraction with d after
 *            the point)
 * Range:     u8 n <= 23, u16 n <= 2078, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The result is returned in lowest terms as a(seq_t). It uses gcd from ../utils/gcd.h. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Count decimal digits of a 64‑bit integer. */
static int digits_uint64(uint64_t x)
{
    int d = 0;
    do
    {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* Compute 10^e as a seq_t.  e ≥ 0. */
static seq_t pow10_seq(int e)
{
    seq_t v = 1;
    while (e-- > 0)
        v *= 10;
    return v;
}

/* The requested sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Find the maximum number of decimal digits among all divisors of n. */
    int maxdigits = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            int ld = digits_uint64(d);
            if (ld > maxdigits)
                maxdigits = ld;
            idx_t d2 = n / d;
            if (d2 != d)
            {
                int ld2 = digits_uint64(d2);
                if (ld2 > maxdigits)
                    maxdigits = ld2;
            }
        }
    }

    /* Common denominator 10^maxdigits. */
    seq_t denom = pow10_seq(maxdigits);

    /* Sum all numerators after scaling to the common denominator. */
    seq_t numer = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            int ld = digits_uint64(d);
            numer += (seq_t)d * pow10_seq(maxdigits - ld);
            idx_t d2 = n / d;
            if (d2 != d)
            {
                int ld2 = digits_uint64(d2);
                numer += (seq_t)d2 * pow10_seq(maxdigits - ld2);
            }
        }
    }

    /* Reduce the fraction numer/denom and return the numerator. */
    seq_t g = gcd(numer, denom);
    return numer / g;
}
