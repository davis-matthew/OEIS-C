/*
 * A276239  a(n) = numerator of rational fraction of function (Gamma[5/4]^2
 *          Gamma[n + 3/4]^2)/(Gamma[3/4]^2 Gamma[n + 5/4]^2).
 * https://oeis.org/A276239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = square of the numerator of ∏_{i=0}^{n-1} (4i+3)/(4i+5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* Return a(n) for n ≥ 0.  The calculation is done by
   multiplying the product ∏ (4i+3)/(4i+5) and simplifying
   the fraction at each step to keep the numbers manageable. */
seq_t a(idx_t n)
{
    seq_t num = 1;   /* numerator of the product */
    seq_t den = 1;   /* denominator of the product */

    for (idx_t i = 0; i < n; ++i) {
        seq_t num_i = 4 * (seq_t)i + 3;   /* 4i + 3 */
        seq_t den_i = 4 * (seq_t)i + 5;   /* 4i + 5 */

        /* Cancel gcd with current denominator */
        seq_t g = gcd(num_i, den);
        if (g) {
            num_i /= g;
            den   /= g;
        }

        /* Cancel gcd with current numerator */
        g = gcd(num, den_i);
        if (g) {
            num   /= g;
            den_i /= g;
        }

        /* Accumulate the reduced factors */
        num *= num_i;
        den *= den_i;
    }

    /* After reduction num/den is in lowest terms.
       The desired value is the square of the numerator. */
    return num * num;
}
