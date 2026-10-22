/*
 * A354303  a(n) is the denominator of Sum_{k=0..n} 1 / (k!)^2.
 * https://oeis.org/A354303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = denominator of Sum_{k=0..n} 1/(k!)^2.
 *
 * Computation: keep the sum as a reduced fraction p/q; when adding
 * 1/d where d = (k!)^2, use gcd cancellation first to avoid
 * overflow on q*d.  The updated fraction is
 *        p' = p * (d/g) + q/g
 *        q' = (q/g) * d
 * and finally reduce again by gcd(p',q').  This keeps all
 * intermediate products within the limits of seq_t as long as the
 * final denominator fits in seq_t (the required behaviour for the
 * 64‑bit build).
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    seq_t p = 0;      /* numerator of the partial sum */
    seq_t q = 1;      /* denominator of the partial sum */
    seq_t fact = 1;   /* k! as we iterate through k */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t d;
        if (k == 0) {
            d = 1;                      /* (0!)^2 */
        } else {
            fact *= (seq_t)k;            /* k! = (k-1)! * k */
            d = fact * fact;             /* (k!)^2 */
        }

        /* Add 1/d to the current fraction p/q */
        seq_t g = seq_gcd(q, d);        /* gcd(q,d) */
        seq_t q1 = q / g;
        seq_t d1 = d / g;

        seq_t new_p = p * d1 + q1;
        seq_t new_q = q1 * d;           /* q/g * d */

        seq_t g2 = seq_gcd(new_p, new_q);
        if (g2 != (seq_t)1) {
            new_p /= g2;
            new_q /= g2;
        }
        p = new_p;
        q = new_q;
    }
    return q;   /* denominator of the reduced sum */
}
