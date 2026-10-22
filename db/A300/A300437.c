/*
 * A300437  Triangle T(nu,m) read by rows: The number of N-color odd
 *          self-inverse compositions of (2*nu+1) into (2*m+1) parts.
 * https://oeis.org/A300437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A300437 - number of N‑color odd self‑inverse compositions of an odd sum
 *
 *  Formula (OEIS entry):
 *      T(ν,m) = Σ_{t=1}^{2ν+1}   Σ_{i+j = (2ν+1-t-2m)/4}
 *                  t · C(2m+i-1, 2m-1) · C(m, j)
 *  where ν ≥ 0, 0 ≤ m ≤ ν.
 *  The sequence a(n) is obtained by reading the triangle T(ν,m) row by row.
 *
 *  Implementation note:
 *      The summation limits are checked with unsigned arithmetic; a
 *      potential negative numerator is detected by testing whether
 *      t + 2m exceeds 2ν+1.  All binomial coefficients are computed
 *      multiplicatively by the utility function binom().
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find row ν and column m from linear index n */
    idx_t ν = 0;
    while ((ν + 1) * (ν + 2) / 2u <= n)
        ++ν;
    idx_t start = ν * (ν + 1) / 2u;
    idx_t m = n - start;

    /* first column T(ν,0) = 2ν+1 */
    if (m == 0)
        return (seq_t)(2 * ν + 1u);

    /* bounds for the summation */
    idx_t two_ν_plus_1 = 2 * ν + 1u;
    idx_t two_m        = 2 * m;

    seq_t sum = 0;

    for (idx_t t = 1; t <= two_ν_plus_1; ++t)
    {
        /* skip when (2ν+1 - t - 2m) < 0 */
        if (t + two_m > two_ν_plus_1)
            continue;

        /* numerator of the division by 4 */
        idx_t numer = two_ν_plus_1 - t - two_m;
        if (numer & 0x3u)          /* not divisible by 4 */
            continue;

        idx_t total = numer >> 2;   /* (2ν+1 - t - 2m)/4 */

        for (idx_t i = 0; i <= total; ++i)
        {
            idx_t j = total - i;
            seq_t a1 = binom(two_m + i - 1u, two_m - 1u);
            seq_t a2 = binom(m, j);
            seq_t prod = a1 * (seq_t)t;
            prod *= a2;
            sum += prod;
        }
    }

    return sum;
}
