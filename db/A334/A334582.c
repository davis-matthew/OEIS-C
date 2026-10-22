/*
 * A334582  Denominator of Sum_{k=1..n} (-1)^(k+1)/k^3.
 * https://oeis.org/A334582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator( Σ_{k=1..n} (-1)^(k+1) / k^3 )
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* Return the denominator of the alternating sum of 1/k^3 up to n */
seq_t a(idx_t n)
{
    /* Initial value 0/1 */
    seq_t num_abs = 0;   /* absolute value of numerator */
    bool   num_neg = false;     /* true if numerator is negative */
    seq_t den     = 1;          /* current denominator, always > 0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Sign of the k‑th term: positive for odd k, negative for even k */
        bool term_neg = ((k & 1) == 0);  /* true if the term is negative */

        /* Compute k^3 in seq_t to avoid overflow of the intermediate 64‑bit value */
        seq_t kk = (seq_t)k;
        seq_t term_den = kk * kk;   /* k^2   */
        term_den *= kk;             /* k^3   */

        /* Find lcm components */
        seq_t g = gcd(den, term_den);
        seq_t term_den_div_g = term_den / g;
        seq_t den_div_g      = den / g;

        /* Numerator parts (magnitudes) */
        seq_t a_mag = num_abs * term_den_div_g; /* |old_num| * (k^3 / g) */
        seq_t b_mag = den_div_g;                /* 1 * (den / g)   */

        /* Combine signed numerators */
        seq_t new_abs;
        bool  new_neg;
        if (num_neg == term_neg) {                 /* same sign → add */
            new_abs = a_mag + b_mag;
            new_neg = term_neg;
        } else {                                   /* opposite signs → subtract */
            if (a_mag >= b_mag) {
                new_abs = a_mag - b_mag;
                new_neg = num_neg;
            } else {
                new_abs = b_mag - a_mag;
                new_neg = term_neg;
            }
        }

        /* New denominator before reduction */
        seq_t lcm_den = den_div_g * term_den; /* lcm(den, k^3) */

        /* Reduce fraction */
        seq_t g2 = gcd(new_abs, lcm_den);
        new_abs /= g2;
        lcm_den /= g2;

        /* Update state */
        num_abs = new_abs;
        num_neg = (new_abs != (seq_t)0) && new_neg; /* zero has no sign */
        den     = lcm_den;
    }

    return den;
}
