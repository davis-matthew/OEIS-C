/*
 * A263295  Denominators of the n-th iteration of the alternating continued
 *          fraction formed from the positive integers, starting with (1 -
 *          ...).
 * https://oeis.org/A263295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 1 - 1/(2 + 1/(3 - 1/(4 + …))) with n
 *            iterations
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Return the denominator of the n‑th iteration of the alternating
   continued fraction formed from the positive integers, starting with (1 - ...). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range calls */

    /* Start from the deepest integer n+1. */
    seq_t p, q;
    if (( (n + 1) & 1 ) != 0) {    /* odd : m - 1 */
        p = (seq_t)(n + 1) - 1;
    } else {                       /* even : m + 1 */
        p = (seq_t)(n + 1) + 1;
    }
    q = 1;

    /* Work upward to i = 2 (the first inner term). */
    for (idx_t i = n; i >= 2; --i) {
        seq_t new_p, new_q;
        if ((i & 1) == 0) {       /* even level:  i + 1/( … ) */
            new_p = (seq_t)i * p + q;
            new_q = p;
        } else {                  /* odd level:   i - 1/( … ) */
            new_p = (seq_t)i * p - q;
            new_q = p;
        }
        seq_t g = gcd(new_p, new_q);   /* reduce to lowest terms */
        new_p /= g;
        new_q /= g;
        p = new_p;
        q = new_q;
    }

    /* After reduction the final denominator of 1 - 1/T₂ is simply p. */
    return p;
}
