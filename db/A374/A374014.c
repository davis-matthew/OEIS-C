/*
 * A374014  Expansion of (Sum_{k>=0} x^(k^4))^16.
 * https://oeis.org/A374014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered decompositions of n into 16 fourth
 *            powers. Computed by 16-fold convolution of the indicator
 *            sequence of fourth powers
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 1846, u64 n <= 9915,
 *            u128 n <= 9915, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Gather all fourth powers k^4 <= n, including k=0. */
    size_t kmax = 0;
    for (idx_t k = 0;; ++k) {
        idx_t ksq   = k * k;
        idx_t kp4   = ksq * ksq;   /* k^4 */
        if (kp4 > n) break;
        ++kmax;
    }

    /* Allocate and fill the power array. */
    idx_t *powers = (idx_t *)malloc(kmax * sizeof(idx_t));
    if (!powers) return 0;  /* allocation failure: return 0 as safe default */
    for (size_t i = 0; i < kmax; ++i) {
        idx_t k   = (idx_t)i;          /* 0,1,2,... */
        idx_t ksq = k * k;
        powers[i] = ksq * ksq;         /* k^4 */
    }

    size_t N = (size_t)n;

    /* Dynamic programming arrays. */
    seq_t *dp_prev = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *dp_cur  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp_prev || !dp_cur) {
        free(powers);
        free(dp_prev);
        free(dp_cur);
        return 0;
    }
    dp_prev[0] = 1;  /* one way to reach sum 0 with 0 summands */

    /* Perform 16 convolutions. */
    for (int iter = 0; iter < 16; ++iter) {
        /* Zero current array. */
        for (size_t i = 0; i <= N; ++i) dp_cur[i] = 0;

        for (size_t i = 0; i <= N; ++i) {
            seq_t ways = dp_prev[i];
            if (ways == 0) continue;
            for (size_t pi = 0; pi < kmax; ++pi) {
                size_t sum = i + (size_t)powers[pi];
                if (sum > N) break;        /* powers are sorted increasingly */
                dp_cur[sum] += ways;
            }
        }

        /* Swap pointers for next iteration. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur = tmp;
    }

    seq_t result = dp_prev[N];

    /* Clean up */
    free(dp_cur);
    free(dp_prev);
    free(powers);

    return result;
}
