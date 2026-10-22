/*
 * A322081  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{d|n} (-1)^(n/d+1)*d^k.
 * https://oeis.org/A322081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 30, i16 n <= 69, i32 n <= 158, i64 n <= 414,
 *            i128 n <= 1138, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth term read along the antidiagonals of the square array
   A(n,k) = Σ_{d|n} (-1)^(n/d+1) * d^k.  The function computes the value
   for any n ≥ 1 using the divisor‑sum formula and the correspondence
   (row,col) derived from the antidiagonal index. */

#include <math.h>               /* sqrt, for anti‑diagonal mapping */
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

/* triangular number: T(d) = d*(d+1)/2 */
static inline idx_t tri(idx_t d) { return d * (d + 1) / 2; }

/* main sequence function */
seq_t a(idx_t n)
{
    /* 0‑based index of the element in the anti‑diagonal enumeration  */
    idx_t m = n - 1;

    /* approximate diagonal d: solve T(d-1) ≤ m < T(d)  */
    idx_t d = (idx_t)((sqrt(8.0 * (double)m + 1.0) - 1.0) / 2.0);

    /* Correct possible off‑by‑one caused by floating‑point rounding  */
    while (tri(d) <= m) ++d;
    while (tri(d - 1) > m) --d;   /* d ≥ 1 */

    idx_t offset   = m - tri(d - 1);   /* position inside diagonal   */
    idx_t row      = offset + 1;       /* 1‑based row index          */
    idx_t col      = (d - 1) - offset; /* 0‑based column index       */

    /* compute Σ_{t|row} (-1)^(row/t+1) * t^col */
    seq_t result = 0;

    for (idx_t t = 1; t * t <= row; ++t) {
        if (row % t == 0) {
            /* divisor t */
            seq_t pow_t  = big_pow((seq_t)t, (seq_t)col);
            seq_t sign   = (((row / t + 1) & 1) ? (seq_t)(-1) : (seq_t)1);
            result += pow_t * sign;

            /* partner divisor row / t, if distinct */
            idx_t partner = row / t;
            if (partner != t) {
                seq_t pow_p  = big_pow((seq_t)partner, (seq_t)col);
                seq_t sign_p = (((row / partner + 1) & 1) ? (seq_t)(-1) : (seq_t)1);
                result += pow_p * sign_p;
            }
        }
    }
    return result;
}
