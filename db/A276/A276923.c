/*
 * A276923  Number of ordered set partitions of [2n] where the maximal block
 *          size equals n.
 * https://oeis.org/A276923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of ordered set partitions of [2n] whose
 *            maximal block size equals n. The implementation uses a(n) =
 *            f(2n, n) – f(2n, n-1), where f(m, k) counts ordered partitions
 *            of m elements into blocks of size at most k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
    /* Base case */
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t m = n * 2;             /* total number of elements          */
    idx_t i, t;
    seq_t *dp1;
    seq_t *dp2;
#ifdef __cplusplus
    /* Use std::vector for C++ to ensure proper construction of big‑int objects */
    std::vector<seq_t> v1(m + 1), v2(m + 1);
    dp1 = v1.data();
    dp2 = v2.data();
#else
    dp1 = (seq_t *)calloc(m + 1, sizeof(seq_t));
    dp2 = (seq_t *)calloc(m + 1, sizeof(seq_t));
#endif

    /* Initialize base values */
    dp1[0] = (seq_t)1;
    dp2[0] = (seq_t)1;
    for (i = 1; i <= m; ++i) {
        dp1[i] = (seq_t)0;
        dp2[i] = (seq_t)0;
    }

    /* Dynamic programming recurrence */
    for (i = 1; i <= m; ++i) {
        seq_t binom = (seq_t)1;          /* C(i,0) */
        idx_t max_t = (i < n) ? i : n;   /* upper bound for block size */

        for (t = 1; t <= max_t; ++t) {
            binom = binom * (seq_t)(i - t + 1) / (seq_t)t;   /* C(i, t) */
            dp1[i] += binom * dp1[i - t];
            if (t < n) {                         /* blocks ≤ n-1 */
                dp2[i] += binom * dp2[i - t];
            }
        }
    }

    seq_t result = dp1[m] - dp2[m];

#ifndef __cplusplus
    free(dp1);
    free(dp2);
#endif
    return result;
}
