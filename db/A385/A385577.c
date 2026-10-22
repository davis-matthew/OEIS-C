/*
 * A385577  Array read by ascending antidiagonals: A(n,m) =
 *          n*Pochhammer(n+1,m+1)/(m+2).
 * https://oeis.org/A385577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained by flattening the array A(n,m) = n *
 *            Pochhammer(n+1,m+1)/(m+2) along ascending antidiagonals (i
 *            decreasing, j increasing)
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Return the diagonal index d (with d>=0) containing position n
   in the flattened sequence.  Positions are zero‑based. */
static idx_t diagonal_of(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = 1;

    /* Find an upper bound hi such that (hi+1)(hi+2)/2 > n */
    while ((unsigned __int128)(hi + 1) * (hi + 2) / 2 <= n)
        hi <<= 1;

    /* Binary search on [lo,hi] for smallest d with (d+1)(d+2)/2 > n */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 val = (unsigned __int128)(mid + 1) * (mid + 2) / 2;
        if (val <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* lo is the required diagonal d */
}

/* Compute A(i,j) = i * Pochhammer(i+1,j+1)/(j+2) with exact arithmetic. */
static seq_t compute_A(idx_t i, idx_t j)
{
    seq_t res = 1;
    seq_t denom = (seq_t)(j + 2);

    /* Multiply over terms i, i+1, ..., i+j+1 while cancelling with denom. */
    for (idx_t t = 0; t <= j + 1; ++t) {
        seq_t term = (seq_t)(i + t);
        seq_t g = gcd(term, denom);
        if (g > 1) {
            denom /= g;
            term   /= g;
        }
        res *= term;
    }

    if (denom != 1)
        res /= denom;   /* Safety – normally denom should be 1 */
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t d = diagonal_of(n);
    /* Cumulative count up to previous diagonal: d*(d+1)/2 */
    unsigned __int128 prev_count = (unsigned __int128)d * (d + 1) / 2;
    idx_t offset = (idx_t)(n - prev_count);

    /* Coordinates in the diagonal: i = d - offset, j = offset */
    idx_t i = d - offset;
    idx_t j = offset;

    return compute_A(i, j);
}
