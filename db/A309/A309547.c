/*
 * A309547  Sum of the even parts in the partitions of n into 5 parts.
 * https://oeis.org/A309547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309547: Sum of the even parts in the partitions of n into 5 parts */

#include <stdlib.h>

/* Computes a(n), the sum of the even parts in all partitions of n into 5 parts. */
seq_t a(idx_t n)
{
    /* Flattened 2‑D arrays: dimension (k+1) × (n+1), 0 ≤ k ≤ 5. */
    const size_t stride = (size_t)n + 1;
    const size_t total  = stride * 6;     /* 6 rows for k = 0..5 */
    seq_t *cnt  = (seq_t *)calloc(total, sizeof(seq_t));
    seq_t *sumEV= (seq_t *)calloc(total, sizeof(seq_t));
    if (!cnt || !sumEV) return 0;        /* allocation failure → 0 */

    /* Base: one way to sum to 0 with 0 parts. */
    cnt[0] = 1;     /* cnt[k*stride + sum] */

    /* Dynamic programming over part sizes. */
    for (idx_t part = 1; part <= n; ++part)
    {
        for (idx_t sum = part; sum <= n; ++sum)
        {
            for (int k = 1; k <= 5; ++k)
            {
                size_t idx_prev = (size_t)(k - 1) * stride + (size_t)(sum - part);
                seq_t prev_cnt  = cnt[idx_prev];
                if (!prev_cnt) continue;

                size_t idx_cur = (size_t)k * stride + (size_t)sum;
                cnt[idx_cur] += prev_cnt;

                seq_t prev_sumEV = sumEV[idx_prev];
                seq_t add = prev_sumEV;
                if ((part & 1) == 0) add += (seq_t)part * prev_cnt;
                sumEV[idx_cur] += add;
            }
        }
    }

    seq_t result = sumEV[5 * stride + (size_t)n];
    free(cnt);
    free(sumEV);
    return result;
}
