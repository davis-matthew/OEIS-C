/*
 * A387718  Smallest divisor d of n such that n/d is deficient, i.e.,
 *          sigma(n/d) < 2*(n/d).
 * https://oeis.org/A387718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = smallest divisor d of n such that σ(n/d) < 2 * (n/d)
 * Range:     u8 n <= 404, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9675 (2026-11-01)
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
    if (n == 0) return 0;                          /* safety guard */
    idx_t d;

    for (d = 1; d <= n; ++d)
    {
        if (n % d != 0) continue;                  /* skip non‑divisors */
        idx_t m = n / d;
        seq_t sigma_m = sigma_of(m);
        seq_t two_m   = (seq_t)2 * (seq_t)m;
        if (sigma_m < two_m)                       /* deficient quotient */
            return (seq_t)d;
    }

    /* In theory d = n always works (m = 1 is deficient) */
    return (seq_t)n;
}
