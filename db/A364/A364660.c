/*
 * A364660  Numerators of coefficients in expansion of (1 + x)^(1/4).
 * https://oeis.org/A364660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 13, i64 n <= 24, i128 n <= 47,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Recurrence: a(0)=1.
   For n>0 let
       g1 = gcd(|a(n‑1)|, n),
       g2 = gcd(|1‑4(n‑1)|, n/g1);
   then
       a(n) = (a(n‑1)/g1) * ((1‑4(n‑1))/g2).
   The fraction (1+x)^(1/4)=∑_{n≥0} a(n)/2^{e_n}x^n has even‑denominator
   powers only, and the numerators a(n) are the terms of A364660. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;   /* a(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* factor (1-4(k)) for k = i‑1 */
        seq_t f = (seq_t)1 - (seq_t)4 * (seq_t)(i - 1);

        /* cancel odd common factors of a_val with the index i */
        seq_t abs_a = a_val;
        if (abs_a < 0)
            abs_a = -abs_a;
        seq_t g1  = gcd(abs_a, (seq_t)i);
        a_val     = a_val / g1;
        seq_t i_div = ((seq_t)i) / g1;   /* remaining part of the index */

        /* cancel remaining odd common factors of f with i_div */
        seq_t abs_f = f;
        if (abs_f < 0)
            abs_f = -abs_f;
        seq_t g2  = gcd(abs_f, i_div);
        f          = f / g2;
        /* the leftover i_div/g2 is a power of two; it will be absorbed
           into the denominator and is thus ignored in the numerator. */

        /* update the numerator */
        a_val = a_val * f;
    }

    return a_val;
}
