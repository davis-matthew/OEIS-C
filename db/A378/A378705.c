/*
 * A378705  Inverse permutation to A378200.
 * https://oeis.org/A378705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,n) where r is such that sum_{i=1}^{r} (4i-3) >= n and
 *            T(r,k) = B + P B = A000384(2r-3) = (2r-3)(2r-2)/2 m = 2r-1 P = {
 *            m-k if k<m and k odd m+k-1 if k<m and k even 3m-k-1 if k>=m and
 *            k odd k-m if k>=m and k even } with k = n - sum_{i=1}^{r-1}
 *            (4i-3) (for n=1 the answer is 1)
 * Range:     u8 n <= 234, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 0, hi = (n >> 1) + 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

static inline idx_t cumulative(idx_t r)
{
    return 2 * r * r - r;            /* sum_{i=1}^{r} (4i-3) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */
    if (n == 1) return 1;

    /* Find row r: smallest r with cumulative(r) >= n */
    idx_t r = (isqrt_u64(1 + 8 * n) + 1) / 4;   /* initial estimate */

    while (cumulative(r) < n) r++;
    while (r > 0 && cumulative(r - 1) >= n) r--;

    idx_t prev = cumulative(r - 1);
    idx_t k = n - prev;                     /* position in the row (1‑based) */

    /* Base of the block: B = A000384(2r-3) = (2r-3)(2r-2)/2 */
    idx_t t = 2 * r - 3;
    seq_t base = ((seq_t)t * (seq_t)(t + 1)) / 2;

    /* P calculation */
    seq_t m = (seq_t)(2 * r - 1);
    seq_t k_s = (seq_t)k;
    seq_t P;

    if (k_s < m) {
        if (k_s & 1)  P = m - k_s;          /* k odd, k < m */
        else          P = m + k_s - 1;       /* k even, k < m */
    } else {
        if (k_s & 1)  P = 3 * m - k_s - 1;   /* k odd, k >= m */
        else          P = k_s - m;           /* k even, k >= m */
    }

    return base + P;
}
