/*
 * A338397  Triangle read by rows: T(n,m)= Sum_{k=0..m/2}
 *          C(n-k,m-2*k)*C(n-k,m-k)*C(n,k)/C(2*k,k).
 * https://oeis.org/A338397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,m) where r = floor((sqrt(8*n+1)-1)/2) // row index
 *            (0‑based) m = n - r*(r+1)/2 // column index in that row T(n,m) =
 *            Σ_{k=0..⌊m/2⌋} C(n-k, m-2k) * C(n-k, m-k) * C(n,k) / C(2k,k) The
 *            sequence is the triangle read by rows
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t floor_sqrt_tri(idx_t n)
{
    /* binary search for r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n + 1;        /* high exclusive */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= (unsigned __int128)n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

static seq_t triangle_value(idx_t n, idx_t m)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= m / 2; ++k) {
        idx_t nk   = n - k;
        idx_t m2k  = m - 2 * k;
        idx_t mk   = m - k;
        seq_t term = binomial(nk, m2k);
        term *= binomial(nk, mk);
        term *= binomial(n, k);
        seq_t denom = binomial(2 * k, k);
        term /= denom;           /* division is exact by construction */
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* find row r and column m for the flattened index n */
    idx_t r = floor_sqrt_tri(n);
    /* compute the cumulative number of elements before row r */
    unsigned __int128 prev = (unsigned __int128)r * (r + 1) / 2;
    idx_t m = n - (idx_t)prev;
    return triangle_value(r, m);
}
