/*
 * A375044  Irregular triangular array T; row n shows the coefficients of the
 *          (n-1)-st polynomial in the obverse convolution s(x)**t(x), where
 *          s(x) = 2^n x and t(x) = x+1. See Comments.
 * https://oeis.org/A375044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A375044
   Row r (r ≥ 1) consists of the coefficients of the degree‑r polynomial

        P_r(x) = ∏_{i=0}^{r-1} (1 + (2^i + 1)x)

   Written in the same order as the OEIS list, the index n is
   distributed as follows:

        T(r) = ½ r(r+3)   (total terms up to row r)

   The term a(n) is the coefficient of x^k in P_r(x) where

        k = n – T(r‑1) – 1   (0‑based index inside the row).
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for offset 1 */

    /* locate the row r such that T(r‑1) < n ≤ T(r), with
       T(r) = r(r+3)/2.  Since n is small (≤ 10⁴) a linear search is
       fast enough. */
    idx_t r = 0;
    while ((r * (r + 3)) / 2 < n) ++r;

    idx_t terms_before = (r - 1) * (r + 2) / 2;   /* T(r‑1) */
    idx_t k = n - terms_before - 1;               /* 0‑based index in row */

    /* allocate array big enough to hold all coefficients 0…r */
    seq_t *poly = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    poly[0] = 1;                                 /* constant term */

    /* Build the polynomial by multiplying successive factors
       (1 + (2^i + 1)x) for i = 0 … r‑1.  */
    for (idx_t i = 0; i < r; ++i) {
        seq_t factor = pow2(i);          /* 2^i  */
        factor = factor + 1;             /* 2^i + 1 */

        seq_t *newc = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
        for (idx_t j = 0; j <= i + 1; ++j) {
            seq_t val = 0;
            if (j <= i)   val = poly[j];
            if (j > 0)    val = val + seq_mul(poly[j - 1], factor);
            newc[j] = val;
        }
        /* copy back to poly */
        for (idx_t j = 0; j <= i + 1; ++j) poly[j] = newc[j];
        free(newc);
    }

    seq_t result = poly[k];
    free(poly);
    return result;
}
