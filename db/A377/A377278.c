/*
 * A377278  Denominators in a harmonic triangle; q-analog of A126615, here q =
 *          2.
 * https://oeis.org/A377278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) = flattening of the triangle T(n,k) where T(n,k) =
 *            (2^k-1)(2^(k+1)-1) for k<n, and T(n,n)=2^n-1
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"

/* Compute 2^exp using multiplication.  Works for any seq_t
   (unsigned integer or big‑integer class). */

/* Return the n-th term of A377278 (indexing starts at 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Locate row r such that r(r-1)/2 < n <= r(r+1)/2. */
    idx_t lo = 1, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    idx_t prev_tri = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = n - prev_tri;                /* 1‑based column */

    if (k == r)   /* main diagonal */
        return pow2(r) - (seq_t)1;
    else
        return (pow2(k) - (seq_t)1) * (pow2(k + 1) - (seq_t)1);
}
