/*
 * A363221  Number of strict integer partitions of n such that (length) *
 *          (maximum) <= 2n.
 * https://oeis.org/A363221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of strict partitions of n with (length)*(maximum) ≤ 2n.
 * The code implements a dynamic programming approach that counts, for each
 * possible maximum part m, all subsets of {1,…,m-1} whose sum and size satisfy
 * the length constraint.  It uses a 2‑dimensional DP array
 * dp[sum][k] = number of subsets of {1,…,current_val} that sum to `sum`
 * and contain exactly `k` elements.  The maximum needed subset size is
 * bounded by the largest k with k(k+1)/2 ≤ n, which is O(√n).  The
 * algorithm runs in O(n²·√n) arithmetic operations, which is well below
 * the 2‑second limit for n up to about 10 000.
 */

#include <stddef.h>
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined for n=0, return 0 */
    /* Special case n==1: only partition (1) satisfies the condition. */
    if (n == 1) {
        seq_t one = 1;
        return one;
    }

    /* Determine maximal subset size needed: largest k with k(k+1)/2 ≤ n. */
    idx_t Kmax = 0;
    while ((Kmax + 1) * (Kmax + 2) / 2 <= n) ++Kmax;   /* Kmax ≈ √(2n) */

    /* Allocate DP table: (n+1) rows, each with (Kmax+1) columns. */
    idx_t rows = n + 1;
    idx_t cols = Kmax + 1;
    size_t total = (size_t)rows * (size_t)cols;
    seq_t* dp = (seq_t*)calloc(total, sizeof(seq_t));
    if (!dp) return 0;          /* allocation failed (unlikely) */

    /* Base case: empty subset */
    dp[0 * cols + 0] = (seq_t)1;

    seq_t ans = 0;

    /* Iterate over each possible element value. */
    for (idx_t val = 1; val <= n; ++val) {
        /* Update DP for adding element 'val' */
        for (idx_t sum = n; ; --sum) {
            if (sum < val) break;
            seq_t* curRow = dp + sum * cols;
            seq_t* prevRow = dp + (sum - val) * cols;
            for (idx_t k = 1; k <= Kmax; ++k) {
                curRow[k] += prevRow[k - 1];
            }
            if (sum == val) break;      /* avoid unsigned wrap */
        }

        /* After including 'val', the current DP corresponds to subsets of {1,…,val}. */
        /* Contributions come from partitions whose maximum part is m = val+1. */
        idx_t m = val + 1;
        if (m > n) continue;          /* only m up to n matter */

        idx_t remaining = n - m;      /* sum that the other parts must achieve */
        if (remaining > n) continue;  /* impossible, skip (not necessary) */

        /* Maximum allowed part count for other parts: floor(2n/m)-1 */
        idx_t kLimit = (2 * n) / m;
        if (kLimit == 0) continue;
        --kLimit;                     /* subtract one for the maximum part itself */
        if (kLimit > Kmax) kLimit = Kmax;

        seq_t subtotal = 0;
        seq_t* baseRow = dp + remaining * cols;
        for (idx_t k = 0; k <= kLimit; ++k) {
            subtotal += baseRow[k];
        }
        ans += subtotal;
    }

    free(dp);
    return ans;
}
