/*
 * A278671  Number of n X 3 0..1 arrays with rows in nondecreasing
 *          lexicographic order and columns in nonincreasing lexicographic
 *          order, but with exactly one mistake.
 * https://oeis.org/A278671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 24, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A278671: Number of n×3 0..1 arrays with rows in
   nondecreasing lexicographic order and columns in nonincreasing
   lexicographic order, but with exactly one mistake.
   Generating function:
      x*(4 - 15*x + 26*x² - 19*x³ + x⁴ + 8*x⁵ - 5*x⁶ + x⁷) / (1 - x)¹²
   From this we obtain a closed form:
      a(n) = Σ_{k=1}^{8} c_k * C(n+11-k, 11)
   with coefficients c_k = { 4, -15, 26, -19, 1, 8, -5, 1 }.
*/

#include <stdint.h>
#include <stddef.h>          /* for size_t */
#include "../utils/binom.h"   /* provides seq_t binom(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* n is 1‑based; for n < 1 we return 0 (outside the defined range) */
    if (n == 0) return 0;

    /* Coefficients for k = 1 .. 8 */
    const int coeffs[8] = { 4, -15, 26, -19, 1, 8, -5, 1 };
    /* Corresponding subtractions from n+11 in the binomial arguments */
    const int ks[8]     = { 1,   2,   3,   4, 5, 6, 7, 8 };

    seq_t result = 0;

    for (size_t i = 0; i < 8; ++i) {
        /* Compute m = n + 11 - ks[i]  (always non‑negative) */
        idx_t m = n + 11 - ks[i];

        /* Binomial is zero for m < 11, skip those cases */
        if (m < 11) continue;

        seq_t bin = binom(m, 11);      /* exact integer binomial */

        if (coeffs[i] >= 0)
            result += bin * (seq_t)coeffs[i];
        else
            result -= bin * (seq_t)(-coeffs[i]);
    }

    return result;
}
