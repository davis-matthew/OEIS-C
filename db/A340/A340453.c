/*
 * A340453  G.f.: Product_{n>=0} (1 - x^(5*n+5))^2 / ( (1 - x^(5*n+1))*(1 -
 *          x^(5*n+4)) ).
 * https://oeis.org/A340453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A340453
   1 + q + q^2 + q^3 + 2*q^4 + q^6 + q^7 + 2*q^8 + q^9 + q^10 + 2*q^12
   + q^13 + 2*q^15 + 2*q^16 + q^18 + q^19 + 2*q^20 + …
   G.f.:  ∏_{n≥0} (1−x^(5n+5))² / ((1−x^(5n+1))·(1−x^(5n+4)))
   This routine evaluates the coefficient a(n). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* work with a single array """
       First build the product of all (1−x^(5k+1))⁻¹·(1−x^(5k+4))⁻¹
       (unlimited parts of size 1 or 4 mod 5). Then multiply
       by each numerator factor (1−2x^d+x^(2d)) for d=5k+5. */
    size_t len = (size_t)n + 1;
    seq_t *P = (seq_t *)malloc(len * sizeof(seq_t));
    if (!P) return 0;                       /* allocation failure */

    for (size_t i = 0; i < len; ++i) P[i] = 0;   /* zero initial array */
    P[0] = 1;                                 /* coefficient of x^0 */

    /* --------- build denominator product --------------- */
    idx_t kmax = (n == 0) ? 0 : (n - 1) / 5;    /* largest k with 5k+1 <= n   */
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t d1 = 5 * k + 1;                  /* part size 1 mod 5          */
        if (d1 <= n) {
            for (idx_t i = d1; i <= n; ++i)
                P[i] = P[i] + P[i - d1];       /* unlimited copies of d1   */
        }
        idx_t d4 = 5 * k + 4;                  /* part size 4 mod 5          */
        if (d4 <= n) {
            for (idx_t i = d4; i <= n; ++i)
                P[i] = P[i] + P[i - d4];       /* unlimited copies of d4   */
        }
    }

    /* --------- apply numerator factors --------------- */
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t d = 5 * k + 5;                   /* d in the numerator      */
        for (idx_t i = n; ; --i) {            /* descending to keep old values */
            seq_t val = P[i];
            if (i >= d)      val = val - (seq_t)2 * P[i - d];
            if (i >= 2 * d)   val = val + P[i - 2 * d];
            P[i] = val;
            if (i == 0) break;
        }
    }

    seq_t result = P[n];
    free(P);
    return result;
}
