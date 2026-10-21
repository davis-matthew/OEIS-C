/*
 * A273167  Numerators of coefficient triangle for expansion of x^(2*n) in
 *          terms of Chebyshev polynomials of the first kind T(2*m, x)
 *          (A127674).
 * https://oeis.org/A273167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac, easy
 * Formula:   a(n) is the numerator of the rational coefficient * R(r,m) =
 *            binom(2*r, r-m) / 2^(2*r-1) for the row r and column m * that
 *            contain the flattened index n. The row and column are given by *
 *            r = floor((sqrt(8*n+1)-3)/2) + 1 * m = n - r*(r+1)/2 * * This
 *            implements the OEIS sequence A273167
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r containing index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    idx_t prev = r * (r + 1) / 2;   /* cumulative count up to row r-1 */
    idx_t m = n - prev;             /* column inside the row (0‑based) */

    /* Row 0 is a special case with a single entry = 1. */
    if (r == 0) {
        return (seq_t)1;
    }

    idx_t k = r - m;                /* binomial argument: choose k */
    seq_t bin = binom(2 * r, k);    /* binomial coefficient */

    /* Count trailing powers of two in the binomial coefficient. */
    int tz = 0;
    seq_t temp = bin;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++tz;
    }

    /* The denominator contributes 2^(2*r-1) powers of two.
       Only the first min(tz, 2*r-1) can be cancelled. */
    idx_t limit = 2 * r - 1;
    idx_t shift = tz;
    if (shift > limit) {
        shift = limit;
    }

    return bin >> shift;
}
