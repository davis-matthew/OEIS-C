/*
 * A348229  Coordination sequence for Wilkinson's 123-circle packing with
 *          respect to a circle of radius 2.
 * https://oeis.org/A348229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0..6} coeff[j] * ((n-j)/2 + 1) where coeff =
 *            {1,6,14,12,7,-2,-2} and the term ((n-j)/2 + 1) is used only when
 *            n-j is even (otherwise 0)
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int coeff[7] = {1, 6, 14, 12, 7, -2, -2};

seq_t a(idx_t n)
{
    int64_t acc = 0;        /* signed accumulator for safety */

    for (idx_t j = 0; j < 7; ++j) {
        if (n < j) break;          /* skip negative indices */
        idx_t rem = n - j;          /* n - j  ≥ 0 */
        if ((rem & 1) == 0) {       /* only even differences contribute */
            idx_t t = rem / 2 + 1;   /* (n-j)/2 + 1  */
            acc += (int64_t)coeff[j] * (int64_t)t;
        }
    }

    /* The result is non‑negative for all n ≥ 0 */
    return (seq_t)acc;
}
