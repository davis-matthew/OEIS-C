/*
 * A338878  Numerators in a set of expansions of the single-term Machin-like
 *          formula for Pi.
 * https://oeis.org/A338878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, tabl
 * Formula:   a(n) = T(row(n), col(n)) where row(n) is the smallest r with
 *            r*(r+1)/2 ≥ n, col(n) = n – (r‑1)*r/2, and T(r,k) = r/gcd(r,
 *            k*(k-1))
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* column index k in that row */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;

    /* compute numerator r / gcd(r, k*(k-1)) */
    seq_t g = gcd((seq_t)r, (seq_t)k * (k - 1));
    seq_t result = (seq_t)r / g;
    return result;
}
