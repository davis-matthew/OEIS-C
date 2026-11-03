/*
 * A336937  The 2-adic valuation of sigma(n), the sum of divisors of n.
 * https://oeis.org/A336937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of 2 in sigma(n) (the sum of divisors of n)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8387 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);   /* compute σ(n) */
    seq_t v2 = 0;                /* 2‑adic valuation of σ(n) */
    while ((sigma & 1) == 0)
    {
        sigma >>= 1;
        v2++;
    }
    return v2;
}
