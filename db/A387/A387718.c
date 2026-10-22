/*
 * A387718  Smallest divisor d of n such that n/d is deficient, i.e.,
 *          sigma(n/d) < 2*(n/d).
 * https://oeis.org/A387718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = smallest divisor d of n such that σ(n/d) < 2 * (n/d)
 * Range:     u8 n <= 404, u16 n <= 9675, u32 n <= 9675, u64 n <= 9675,
 *            u128 n <= 9675, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

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
