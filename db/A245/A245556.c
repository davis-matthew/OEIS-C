/*
 * A245556  Irregular triangle read by rows: T(n,k) (n>=0, 0 <= k <= 2n) =
 *          number of triples (u,v,w) with entries in the range 0 to n which
 *          have some pair adding up to k.
 * https://oeis.org/A245556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(floor_sqrt(n), n - floor_sqrt(n)^2), where T(r,k) is
 *            the number of ordered triples (u,v,w) with 0 ≤ u,v,w ≤ r such
 *            that at least one of u+v, u+w, v+w equals k. For 0 ≤ k ≤ 2r, let
 *            low = (k > r) ? k - r : 0, high = (k < r) ? k : r, count2 =
 *            max(0, high - low + 1), cnt3 = ((k & 1) == 0 &&...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for uint64_t, floor(sqrt(x)) */
static inline idx_t isqrt_u64(idx_t x) {
    if (x < 2)
        return x;
    idx_t lo = 1;
    idx_t hi = (x < 4294967296ULL) ? x : 4294967295ULL; /* sqrt(2^64-1) = 2^32-1 */
    idx_t ans = 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid <= x / mid) {            /* mid*mid <= x without overflow */
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    idx_t r = isqrt_u64(n);                /* row index */
    idx_t r_sq = r * r;
    idx_t k = n - r_sq;                    /* column index */

    /* compute count2 */
    idx_t low = (k > r) ? (k - r) : 0;
    idx_t high = (k < r) ? k : r;
    idx_t count2_idx = (high >= low) ? (high - low + 1) : 0;
    seq_t count2 = (seq_t)count2_idx;

    seq_t N = (seq_t)(r + 1);              /* number of possible values for each variable */

    seq_t A = count2 * N;                  /*  count of triples satisfying u+v = k  (and similarly for other pairs) */

    /* triple intersection: only when k even and k/2 <= r */
    seq_t cnt3 = ((k & 1ULL) == 0 && ((k >> 1) <= r)) ? 1 : 0;

    /* inclusion–exclusion to get T(r,k) */
    seq_t result = 3 * A - 3 * count2 + cnt3;
    return result;
}
