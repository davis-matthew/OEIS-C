/*
 * A357846  Denominators of the partial alternating sums of the reciprocals of
 *          the sum of divisors function (A000203).
 * https://oeis.org/A357846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{k=1..n} (-1)^(k+1)/sigma(k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

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
    seq_t num = 0;   /* numerator of the partial sum, always non‑negative */
    seq_t den = 1;   /* denominator of the partial sum */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sigma = sigma_of(k);

        /* new fraction: num/den  +  (-1)^(k+1) / sigma  */
        seq_t new_num, new_den;
        if (k & 1U)            /* odd k → add 1/sigma */
            new_num = num * sigma + den;
        else                  /* even k → subtract 1/sigma */
            new_num = num * sigma - den;

        new_den = den * sigma;

        /* Reduce the fraction: divide numerator and denominator by their GCD */
        seq_t g = gcd(new_num < 0 ? -new_num : new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return den;
}
