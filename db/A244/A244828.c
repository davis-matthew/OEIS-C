/*
 * A244828  Number of length n 0..4 arrays with each partial sum starting from
 *          the beginning no more than two standard deviations from its mean.
 * https://oeis.org/A244828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays of values 0–4 whose every *
 *            partial sum Sₖ satisfies * |Sₖ – 2k| ≤ 2·√(2k) , * i.e. 2k‑t ≤
 *            Sₖ ≤ 2k+t where t = ⌊√(8k)⌋
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer square root of an unsigned 64‑bit integer.  */
static inline idx_t isqrt_u64(idx_t x)
{
    if (x == 0) return 0;
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 == x) return mid;
        else if (mid2 < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* sequence is undefined below offset 1 */

    /* maximum possible sum for length n */
    idx_t maxSum = 4 * n;

    /* allocate two arrays for dynamic programming */
    seq_t *dp_prev = (seq_t *)calloc(maxSum + 1, sizeof(seq_t));
    if (!dp_prev) return 0;
    seq_t *dp_curr = (seq_t *)calloc(maxSum + 1, sizeof(seq_t));
    if (!dp_curr) { free(dp_prev); return 0; }

    dp_prev[0] = 1;                      /* empty prefix has sum 0 */
    idx_t prev_lower = 0, prev_upper = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* bounds for partial sum after k elements */
        idx_t two_k = 2 * k;
        idx_t t = isqrt_u64(8 * k);            /* floor(2·√(2k)) = ⌊√(8k)⌋ */

        idx_t lower = two_k > t ? two_k - t : 0;
        if (lower > maxSum) lower = maxSum;
        idx_t upper = two_k + t;
        if (upper > maxSum) upper = maxSum;

        /* clear current DP slice */
        for (idx_t i = lower; i <= upper; ++i) dp_curr[i] = 0;

        /* transition from previous prefix sums */
        for (idx_t s_prev = prev_lower; s_prev <= prev_upper; ++s_prev) {
            seq_t val = dp_prev[s_prev];
            if (!val) continue;
            /* a can be 0..4 */
            for (idx_t a = 0; a <= 4; ++a) {
                idx_t s_new = s_prev + a;
                if (s_new >= lower && s_new <= upper) {
                    dp_curr[s_new] += val;
                }
            }
        }

        /* swap arrays and update range */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
        prev_lower = lower;
        prev_upper = upper;
    }

    /* total number of admissible sequences after all prefixes */
    seq_t total = 0;
    for (idx_t i = prev_lower; i <= prev_upper; ++i) {
        total += dp_prev[i];
    }

    free(dp_prev);
    free(dp_curr);
    return total;
}
