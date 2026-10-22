/*
 * A348141  a(n) is the denominator of tan(n * arctan(1/n)).
 * https://oeis.org/A348141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of tan(n * arctan(1/n))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 9, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: use the recurrence for tangent addition:
 * if tan(kθ) = A/B (in lowest terms) and tanθ = 1/n,
 * then tan((k+1)θ) = (A·n + B) / (B·n - A); reduce each step. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t a_val = 1;          /* numerator of tan(θ) */
    seq_t b_val = n_seq;      /* denominator of tan(θ) */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t a_old = a_val;
        /* compute next numerator and denominator */
        a_val = a_old * n_seq + b_val;
        b_val = b_val * n_seq - a_old;
        /* reduce fraction */
        seq_t g = gcd(a_val, b_val);
        if (g != 1) {
            a_val /= g;
            b_val /= g;
        }
    }
    return b_val;   /* denominator of tan(n·arctan(1/n)) */
}
