/*
 * A244896  Number of length n 0..1 arrays with each partial sum starting from
 *          the beginning no more than sqrt(2) standard deviations from its
 *          mean.
 * https://oeis.org/A244896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings (0/1 array) of length n * such
 *            that for every k (1 ≤ k ≤ n) the partial sum S_k satisfies *
 *            |S_k – k/2| ≤ sqrt(k/2). Equivalently, * (2S_k - k)^2 ≤ 2k. The
 *            routine computes this value exactly * for any n ≥ 1 using
 *            dynamic programming. * The algorithm iterates over...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static inline idx_t isqrt_u64(idx_t n)
{
    /*  Return ⌊√n⌋.  n is at most 2·n ≤ 20000 for the use in this
     *  routine, so the loop runs very few iterations.  */
    idx_t r = 0;
    while (1) {
        idx_t t = r + 1;
        if ((unsigned __int128)t * t > (unsigned __int128)n)
            break;
        r = t;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* Allocate two DP arrays of length n+1.  */
    size_t size = (size_t)(n + 1);
    seq_t *dp_prev = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *dp_curr = (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp_prev || !dp_curr) {
        free(dp_prev); free(dp_curr);
        return (seq_t)0;
    }

    dp_prev[0] = (seq_t)1;          /* S_0 = 0 has one way */
    idx_t prev_low = 0, prev_high = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* Determine allowed range for partial sum after k steps.  */
        idx_t two_k   = k + k;               /* 2k  */
        idx_t sigma   = isqrt_u64(two_k);    /* ⌊√(2k)⌋ */

        /* d = 2S_k - k must satisfy |d| ≤ σ and share parity with k. */
        idx_t d_min = (idx_t)(-(idx_t)sigma);
        if ( ((d_min & 1u) != (k & 1u)) ) d_min++;   /* adjust parity */
        idx_t d_max = sigma;
        if ( ((d_max & 1u) != (k & 1u)) ) d_max--;   /* adjust parity */

        idx_t low  = (k + d_min) / 2;               /* ceil((k - σ)/2) */
        idx_t high = (k + d_max) / 2;               /* floor((k + σ)/2) */

        memset(dp_curr, 0, size * sizeof(seq_t));

        for (idx_t s = low; s <= high; ++s) {
            seq_t val = 0;
            if (s >= prev_low && s <= prev_high)
                val += dp_prev[s];
            if (s > 0 && (s - 1) >= prev_low && (s - 1) <= prev_high)
                val += dp_prev[s - 1];
            dp_curr[s] = val;
        }

        /* Swap DP arrays for next step. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
        prev_low = low;
        prev_high = high;
    }

    /* Sum all counts for the last step (only positions low..high are non‑zero). */
    seq_t ans = (seq_t)0;
    for (idx_t s = prev_low; s <= prev_high; ++s)
        ans += dp_prev[s];

    free(dp_prev);
    free(dp_curr);
    return ans;
}
