/*
 * A392686  a(n) = denominator( (1/(4*n + 2))*Sum_{i=0..2*n} (-1)^i/(2*i+1) ).
 * https://oeis.org/A392686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator( (1/(4*n + 2)) * Sum_{i=0..2*n}
 *            (-1)^i/(2*i+1) )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* numerator and denominator of the partial sum S(n)
       sign holds the current sign of S(n) (+1 or -1) */
    seq_t num = 1;          /* |S(0)| = 1/1 */
    seq_t den = 1;
    int   sign = 1;         /* S(0) > 0 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t delta_den = (seq_t)(4*k - 1) * (seq_t)(4*k + 1);  /* (4k-1)*(4k+1) */
        seq_t delta_num = 2;                                 /* the -2 in the update */

        /* left = |S| * delta_den
           right = delta_num * den  */
        seq_t left  = num * delta_den;
        seq_t right = delta_num * den;

        seq_t new_num_abs;
        int   new_sign;
        seq_t new_den = den * delta_den;

        if (sign == 1) {
            /* numerator = left - right (may become negative) */
            if (left >= right) {
                new_num_abs = left - right;
                new_sign    = 1;
            } else {
                new_num_abs = right - left;
                new_sign    = -1;
            }
        } else {   /* sign == -1  */
            /* numerator = -(left + right)  */
            new_num_abs = left + right;
            new_sign    = -1;
        }

        /* zero case: set to 0/1 */
        if (new_num_abs == 0) {
            num   = 0;
            den   = 1;
            sign  = 1;
            continue;
        }

        /* reduce the fraction */
        seq_t g = gcd(new_num_abs, new_den);
        num  = new_num_abs / g;
        den  = new_den      / g;
        sign = new_sign;
    }

    /* Final fraction: S(n) / (4*n + 2)  = sign*num / (den*(4*n+2))   */
    seq_t denom_product = den * (seq_t)(4*n + 2);
    seq_t g = gcd(num, denom_product);   /* gcd ignores the sign */
    return denom_product / g;
}
