/*
 * A248801  Number of sets of nonzero squares with sum <= n.
 * https://oeis.org/A248801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of sets of non‑zero squares with sum <= n
 * Range:     u8 n <= 133, u16 n <= 756, u32 n <= 4854, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming counting subsets of squares,
   O(n * sqrt(n)) per call. */

#include <stdlib.h>
#include <stddef.h>

/* Integer square root for idx_t, safe for 64‑bit values */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Allocate DP array for sums 0..n */
    seq_t *f = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!f) return 0;                  /* allocation failed */

    /* Initialize */
    for (idx_t i = 0; i <= n; ++i) f[i] = 0;
    f[0] = 1;

    /* Compute all subset counts for each square weight */
    idx_t kmax = isqrt_idx(n);
    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t w = k * k;                /* weight of square */
        for (idx_t s = n; s >= w; --s) {
            f[s] = f[s] + f[s - w];
        }
    }

    /* Sum up all counts for sums <= n to get a(n) */
    seq_t ans = 0;
    for (idx_t s = 0; s <= n; ++s) ans = ans + f[s];

    free(f);
    return ans;
}
