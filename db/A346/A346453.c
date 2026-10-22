/*
 * A346453  The array in A054238 read by upward antidiagonals.
 * https://oeis.org/A346453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = Morton(i,j) where d = floor((sqrt(8*n+1)-1)/2), offset =
 *            n - d*(d+1)/2, i = d - offset, j = offset
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* integer square‑root using binary search (returns floor(sqrt(x))) */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (1ULL << 32);           /* high is > sqrt(max 64‑bit) */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        /* use 128‑bit to avoid overflow */
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq <= (__int128)x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* 1. Locate the diagonal d containing n */
    idx_t t = isqrt_u64(8 * n + 1);   /* floor(sqrt(8*n+1)) */
    idx_t d = (t - 1) >> 1;           /* floor((sqrt(8*n+1)-1)/2) */

    /* 2. Compute offsets on the diagonal */
    idx_t prev = d * (d + 1) / 2;     /* number of terms before diagonal d */
    idx_t offset = n - prev;

    /* 3. Convert to coordinates (i,j) */
    idx_t i = d - offset;             /* row index */
    idx_t j = offset;                 /* column index */

    /* 4. Interleave bits of j (even positions) and i (odd positions) */
    seq_t val = 0;
    idx_t shift = 0;
    while (i > 0 || j > 0) {
        val |= ((seq_t)(j & 1) << shift);   /* even bit from j */
        shift++;
        val |= ((seq_t)(i & 1) << shift);   /* odd  bit from i */
        shift++;
        j >>= 1;
        i >>= 1;
    }
    return val;
}
