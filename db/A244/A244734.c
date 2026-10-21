/*
 * A244734  Numerators of the triangle T(n,k) = (n*(n+1)/2 + k + 1)/(k+1) for
 *          n >= k >= 0.
 * https://oeis.org/A244734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (T + k + 1) / gcd(T + k + 1, k + 1) where T = r*(r+1)/2
 *            is the r‑th triangular number, r is the row index such that
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, and k = n - r*(r+1)/2 is the
 *            column index within that row. The fraction is reduced before
 *            taking the numerator
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Determine the row r that contains the n-th term */
    long double tmp = sqrtl((long double)8 * (long double)n + 1);
    idx_t r = (idx_t)((tmp - 1.0L) / 2.0L);

    /* Adjust r in case of floating‑point round‑off */
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    while (r * (r + 1) / 2 > n) r--;

    /* Column index within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute T = r*(r+1)/2 as seq_t */
    seq_t T = (seq_t)r * ((seq_t)r + 1) / 2;

    /* Numerator before reduction */
    seq_t num = T + (seq_t)k + 1;
    /* Denominator before reduction */
    seq_t den = (seq_t)k + 1;

    /* Reduce the fraction */
    seq_t g = seq_gcd(num, den);

    return num / g;
}
