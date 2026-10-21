/*
 * A244829  Number of length n 0..5 arrays with each partial sum starting from
 *          the beginning no more than two standard deviations from its mean.
 * https://oeis.org/A244829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244829:
   Number of length‑n arrays with entries 0..5 such that every partial sum
   S_k satisfies |S_k – 2.5 k| ≤ 2·σ_k where σ_k = sqrt(k·35/12).
   The function computes the values by dynamic programming over the range
   of admissible partial sums. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <new>
#endif

/* Integer square root floor of a 64‑bit unsigned integer. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid != 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

/* Compute the lower and upper bounds for partial sums of length i. */
static inline void bounds_for_idx(idx_t i, idx_t *low, idx_t *high) {
    /* L = floor(sqrt((140 * i) / 3)) */
    idx_t L = isqrt_u64((140ULL * i) / 3ULL);

    /* Parity of 5*i (0 or 1). */
    idx_t parity = (5ULL * i) & 1ULL;

    int64_t minD = -(int64_t)L;
    if ((minD & 1LL) != (int64_t)parity) ++minD;      /* adjust to correct parity */
    int64_t maxD =  (int64_t)L;
    if ((maxD & 1LL) != (int64_t)parity) --maxD;

    /* S = (5*i + D)/2   (D has same parity as 5*i) */
    *low  = (idx_t)((5ULL * i + (idx_t)minD) / 2ULL);
    *high = (idx_t)((5ULL * i + (idx_t)maxD) / 2ULL);
}

/* Allocate an array of seq_t and set it to zero.  Works in C and C++. */
static seq_t *alloc_seq_array(size_t n) {
#ifdef __cplusplus
    seq_t *p = new seq_t[n]();   /* value‑initialize to zero */
    return p;
#else
    seq_t *p = (seq_t*)calloc(n, sizeof(seq_t));
    return p;
#endif
}

/* Free an array allocated by alloc_seq_array. */
static void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset starts at 1 */

    /* Step 1 initialization */
    idx_t low, high;
    bounds_for_idx(1, &low, &high);
    size_t width_prev = (size_t)(high - low + 1);
    seq_t *dp_prev = alloc_seq_array(width_prev);
    for (size_t i = 0; i < width_prev; ++i) dp_prev[i] = 1;
    idx_t prev_low = low;
    idx_t prev_high = high;

    /* Iterate from step 2 to n */
    for (idx_t step = 2; step <= n; ++step) {
        bounds_for_idx(step, &low, &high);
        size_t width_cur = (size_t)(high - low + 1);
        seq_t *dp_cur = alloc_seq_array(width_cur);

        /* Prefix sums of dp_prev for fast range queries */
        size_t pref_len = width_prev + 1;
        seq_t *pref = alloc_seq_array(pref_len);
        pref[0] = 0;
        for (size_t i = 0; i < width_prev; ++i)
            pref[i + 1] = pref[i] + dp_prev[i];

        /* Compute dp_cur values */
        for (size_t idx = 0; idx < width_cur; ++idx) {
            idx_t s = low + (idx_t)idx;

            /* t1 = max(prev_low, s-5)   t2 = min(prev_high, s) */
            idx_t t1 = prev_low;
            if (s > 5) {
                idx_t s_m5 = s - 5;
                if (s_m5 > t1) t1 = s_m5;
            }
            idx_t t2 = prev_high;
            if (s < t2) t2 = s;

            if (t1 > t2) {
                dp_cur[idx] = 0;
            } else {
                size_t l = (size_t)(t1 - prev_low);
                size_t r = (size_t)(t2 - prev_low);
                dp_cur[idx] = pref[r + 1] - pref[l];
            }
        }

        free_seq_array(pref);
        free_seq_array(dp_prev);
        dp_prev = dp_cur;
        width_prev = width_cur;
        prev_low = low;
        prev_high = high;
    }

    /* Sum all valid sequences of length n */
    seq_t result = 0;
    for (size_t i = 0; i < width_prev; ++i)
        result += dp_prev[i];

    free_seq_array(dp_prev);
    return result;
}
