/*
 * A244939  Number of length n 0..7 arrays with each partial sum starting from
 *          the beginning no more than sqrt(3) standard deviations from its
 *          mean.
 * https://oeis.org/A244939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of length n 0..7 arrays such that for each
 * partial sum S_k (k = 1..n) the inequality
 *      |S_k - 7k/2| <= sqrt(63k)/2
 * holds.  This is equivalent to the “no more than √3 standard deviations
 * from its mean” condition of A244939.
 *
 * The function computes the count by dynamic programming:
 *   dp_k[s] = number of sequences of length k having partial sum s.
 * The state is limited to the admissible range [S_min(k), S_max(k)], which
 * grows roughly as 8 * √k.  For n ≤ 10000 the program runs in well under
 * two seconds.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t idx_t;   /* supplied by the build environment */

/* Integer square root: floor(sqrt(n)) */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid * mid <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    /* Pre‑compute admissible sum ranges for each k (0..n) */
    idx_t *S_min = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    idx_t *S_max = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    S_min[0] = 0;
    S_max[0] = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = 63 * k;
        idx_t s = isqrt_u64(t);                 /* floor(√(63k)) */
        S_min[k] = (7 * k - s + 1) >> 1;        /* ceil((7k - s)/2) */
        S_max[k] = (7 * k + s) >> 1;            /* floor((7k + s)/2) */
    }

    /* Dynamic programming */
    idx_t width_prev = 1;
    seq_t *dp_prev = (seq_t *)calloc(width_prev, sizeof(seq_t));
    dp_prev[0] = 1;   /* only sum = 0 at step 0 */

    for (idx_t step = 1; step <= n; ++step) {
        idx_t mn = S_min[step];
        idx_t mx = S_max[step];
        size_t width = (size_t)(mx - mn + 1);
        seq_t *dp_next = (seq_t *)calloc(width, sizeof(seq_t));

        idx_t old_min = S_min[step - 1];
        for (size_t i = 0; i < width_prev; ++i) {
            seq_t cnt = dp_prev[i];
            if (cnt == 0) continue;
            idx_t s = old_min + (idx_t)i;
            for (int v = 0; v < 8; ++v) {
                idx_t ns = s + (idx_t)v;
                if (ns < mn || ns > mx) continue;
                dp_next[ns - mn] += cnt;
            }
        }

        free(dp_prev);
        dp_prev = dp_next;
        width_prev = width;
    }

    /* Sum of all end states gives the desired count */
    seq_t total = 0;
    for (size_t i = 0; i < width_prev; ++i) {
        total += dp_prev[i];
    }

    free(dp_prev);
    free(S_min);
    free(S_max);
    return total;
}
