/*
 * A358294  Triangle read by rows: T(n,k) = n^3*k^3*(n+k)^2, n>=0, 0 <= k <=
 *          n.
 * https://oeis.org/A358294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r^3*k^3*(r+k)^2, where (r,k) is the n‑th element in a
 *            triangle read by rows: T(r,k) = r^3 * k^3 * (r + k)^2, r >= 0, 0
 *            <= k <= r. The mapping from index n to (r,k) is r = floor(
 *            (floor_sqrt(8*n + 1) - 1) / 2 ), k = n - r*(r+1)/2
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned 64‑bit values, safe from overflow */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v, ans = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid == 0) {
            ans = 0;
            lo = 1;
            continue;
        }
        /* avoid overflow: mid*mid <= v  iff  mid <= v / mid */
        if (mid <= v / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Determine the row r that contains the nth element. */
    idx_t inside = 8*n + 1;        /* 8*n + 1 fits in idx_t for all n up to 10 000 */
    idx_t s = isqrt_u64(inside);   /* floor(sqrt(8*n+1)) */
    idx_t r = (s - 1) / 2;

    /* Index of the first element of row r. */
    idx_t prev_cum = r * (r + 1) / 2;
    idx_t k = n - prev_cum;        /* column within the row */

    /* Compute T(r,k) = r^3 * k^3 * (r + k)^2 in seq_t */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    seq_t sum = rr + kk;
    seq_t rr2 = rr * rr;
    seq_t rr3 = rr2 * rr;
    seq_t kk2 = kk * kk;
    seq_t kk3 = kk2 * kk;
    seq_t sum2 = sum * sum;
    return rr3 * kk3 * sum2;
}
