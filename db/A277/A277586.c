/*
 * A277586  Numerator of Sum_{k=0..n} (2^k * (k!)^2)/(2k + 1)!.
 * https://oeis.org/A277586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( ∑_{k=0..n} (2^k⋅(k!)^2)/(2k+1)! ) The value
 *            b(n) = ∑_{k=0..n} … satisfies the recurrence b(0) = 1 b(k) = 1 +
 *            k/(2k+1) ⋅ b(k‑1) (for k ≥ 1) The numerator a(n) is the
 *            numerator of b(n) in lowest terms
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = 1;   /* numerator of current b(k) */
    seq_t den = 1;   /* denominator of current b(k) */

    for (idx_t k = n; k > 0; --k)
    {
        /* Compute f = k * num */
        seq_t f = (seq_t)k * num;

        /* Compute g = (2*k + 1) * den */
        seq_t g = ( ( (seq_t)2 * (seq_t)k ) + 1 ) * den;

        /* New fraction b(k) = (g + f) / g  */
        seq_t new_num = g + f;
        seq_t new_den = g;

        /* Reduce to lowest terms */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num; /* numerator of b(n) */
}
