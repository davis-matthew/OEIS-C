/*
 * A280718  Expansion of (Sum_{k>=0} x^(k*(3*k-1)/2))^5.
 * https://oeis.org/A280718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 5‑tuples of pentagonal numbers summing
 *            to n
 * Range:     u8 n <= 74, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Pre‑compute pentagonal numbers ≤ n */
    idx_t k = 0, cnt = 0;
    while (1) {
        idx_t p = k * (3 * k - 1) / 2;
        if (p > n) break;
        ++cnt;
        ++k;
    }

    idx_t *pent = (idx_t *)malloc(cnt * sizeof(idx_t));
    k = 0;
    for (idx_t i = 0; i < cnt; ++i) {
        pent[i] = k * (3 * k - 1) / 2;
        ++k;
    }

    /* Dynamic programming arrays */
    seq_t *dp_prev = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *dp_curr = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));

    dp_prev[0] = 1;

    for (int step = 1; step <= 5; ++step) {
        for (idx_t sum = 0; sum <= n; ++sum) {
            seq_t total = 0;
            for (idx_t j = 0; j < cnt; ++j) {
                idx_t p = pent[j];
                if (p > sum) break;
                total += dp_prev[sum - p];
            }
            dp_curr[sum] = total;
        }
        /* swap and reset for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
        memset(dp_curr, 0, ((size_t)n + 1) * sizeof(seq_t));
    }

    seq_t result = dp_prev[n];

    free(dp_prev);
    free(dp_curr);
    free(pent);

    return result;
}
