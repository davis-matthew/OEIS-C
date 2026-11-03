/*
 * A391176  Expansion of g^3/(1 - x*g)^4, where g = 1+x*g^3 is the g.f. of
 *          A001764.
 * https://oeis.org/A391176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+3) * binomial(k+3,3) *
 *            binomial(3*n-2*k+3,n-k)/(3*n-2*k+3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* small part: (k+3)*C(k+3,3)  (fits in 64‑bit for n up to 10000) */
        seq_t small = (seq_t)(k + 3) * binomial(k + 3, 3);

        /* big part: C(3*n-2*k+3, n-k) */
        seq_t big = binomial(3 * n - 2 * k + 3, n - k);

        /* denominator */
        seq_t denom = (seq_t)(3 * n - 2 * k + 3);

        /* term = small * big / denom  (exact division) */
        seq_t term = (small * big) / denom;

        sum += term;
    }
    return sum;
}
