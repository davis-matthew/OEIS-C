/*
 * A326530  Sum of the third largest parts of the partitions of n into 9
 *          squarefree parts.
 * https://oeis.org/A326530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum over partitions of n into 9 squarefree parts of the
 *            3rd largest part
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming counting partitions (unordered) with part count fixed,
   while accumulating the sum of the third largest part. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n < 9 it is impossible to have 9 positive parts */
    if (n < 9) {
        return (seq_t)0;
    }

    /* Convert to native int for array indexing (n <= 10000 in practice) */
    int N = (int)n;

    /* --------- Sieve squarefree numbers up to N --------- */
    char *is_sq = (char *)calloc(N + 1, 1);  /* 1 = squarefree, 0 = not */
    for (int i = 1; i <= N; ++i) {
        is_sq[i] = 1;
    }
    for (int p = 2; (int64_t)p * p <= N; ++p) {
        int sq = p * p;
        for (int j = sq; j <= N; j += sq) {
            is_sq[j] = 0;
        }
    }

    /* Build list of squarefree values */
    int sf_cnt = 0;
    for (int i = 1; i <= N; ++i) {
        if (is_sq[i]) ++sf_cnt;
    }
    int *sq_vals = (int *)malloc(sf_cnt * sizeof(int));
    int pos = 0;
    for (int i = 1; i <= N; ++i) {
        if (is_sq[i]) {
            sq_vals[pos++] = i;
        }
    }
    free(is_sq);

    /* --------- DP arrays: counts and sum of 3rd largest part --------- */
    const int PARTS = 9;
    size_t stride = (size_t)(N + 1);
    size_t total = (size_t)(PARTS + 1) * stride;   /* indices 0..9 */
    seq_t *dp_cnt = (seq_t *)calloc(total, sizeof(seq_t));
    seq_t *dp_sum = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dp_cnt || !dp_sum) {
        /* Inadequate memory, return 0 (should not happen in normal tests) */
        free(sq_vals);
        return (seq_t)0;
    }

    dp_cnt[0] = 1;  /* base: 0 parts sum to 0 */

    /* Transition: for each squarefree coin value v */
    for (int vi = 0; vi < sf_cnt; ++vi) {
        int v = sq_vals[vi];
        for (int p = 1; p <= PARTS; ++p) {
            seq_t *cnt_prev = dp_cnt + (p - 1) * stride;
            seq_t *cnt_cur  = dp_cnt + p * stride;
            seq_t *sum_prev = dp_sum + (p - 1) * stride;
            seq_t *sum_cur  = dp_sum + p * stride;

            for (int s = v; s <= N; ++s) {
                seq_t old_cnt = cnt_prev[s - v];
                if (!old_cnt) continue;
                seq_t old_sum = sum_prev[s - v];
                /* Update count of partitions */
                cnt_cur[s] += old_cnt;

                /* Update sum of 3rd largest part */
                if (p == 7) {
                    /* The new part becomes the 7th (third largest) part */
                    sum_cur[s] += old_sum + old_cnt * (seq_t)v;
                } else if (p > 7) {
                    /* 3rd largest already fixed earlier */
                    sum_cur[s] += old_sum;
                } else {
                    /* p < 7, still no 3rd largest part yet */
                    /* nothing to add */
                }
            }
        }
    }

    seq_t result = dp_sum[PARTS * stride + N];

    /* Clean up */
    free(dp_cnt);
    free(dp_sum);
    free(sq_vals);
    return result;
}
