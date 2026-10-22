/*
 * A382252  Triangle T(n,k) = numerator of (n+k)/(1+n*k), 0 <= k <= n >= 0,
 *          read by rows.
 * https://oeis.org/A382252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = T(i,j) where * i = floor((sqrt(8 n + 1) - 1) / 2) (row
 *            index) * j = n - i*(i+1)/2 (column index) * T(i,j) = numerator
 *            of (i+j)/(1 + i*j) * This reads the triangle of numerators
 *            row‑major
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"   /* provides gcd(seq_t a, seq_t b) */

static inline idx_t isqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > x / mid) {
            hi = mid - 1;
        } else {
            if (mid + 1 > 0 && mid + 1 > x / (mid + 1))
                return mid;
            lo = mid + 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Determine the row i of the index */
    idx_t root = isqrt(8 * n + 1);
    idx_t i = (root - 1) / 2;

    /* Determine the column j within that row */
    idx_t before = i * (i + 1) / 2;
    idx_t j = n - before;

    /* Numerator and denominator of (i+j)/(1 + i*j) */
    seq_t num = (seq_t)(i + j);
    seq_t den = (seq_t)1 + (seq_t)i * (seq_t)j;

    /* Reduce the fraction and return the numerator */
    seq_t g = gcd(num, den);
    return num / g;
}
