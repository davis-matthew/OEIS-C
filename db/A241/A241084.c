/*
 * A241084  Sum of the second largest parts of the partitions of 4n into 4
 *          parts.
 * https://oeis.org/A241084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A241084
 * Formula: a(n) = sum of the second largest parts of all partitions of 4n into 4 parts.
 * Derived DP: count all non‑negative integer solutions (t,u,v,w) of
 * 4t + 3u + 2v + w = 4(n-1).  For each solution the second part is 1 + t+u+v,
 * hence a(n) = (∑(t+u+v)) + (number of solutions).
 * The arrays cnt_arr and sum_arr store, for each weight k,
 *   cnt_arr[k] = number of (t,u,v) with weight k,
 *   sum_arr[k] = sum of t+u+v over those solutions.
 * Cumulative sums cumC and cumS allow fast retrieval for all weights ≤S.
 *
 * Complexity: O(4(n-1)) time per new maximum n, memory ~ 5*(4n) words, well
 * below limits for n ≤ 10000.  Works for all supported seq_t types.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */

#ifndef MAX_W
#define MAX_W 80000   /* sufficient for n up to ~20000 */
#endif

static idx_t      max_n   = 0;
static seq_t      cnt_arr[MAX_W + 1];
static seq_t      sum_arr[MAX_W + 1];
static seq_t      cumC   [MAX_W + 1];
static seq_t      cumS   [MAX_W + 1];

seq_t a(idx_t n) {
    /* n >= 1 */
    idx_t target = 4 * (n - 1);   /* maximum weight for this n */

    if (n > max_n) {
        /* recompute arrays up to the new maximum */
        max_n = n;
        /* clear up to target */
        for (idx_t i = 0; i <= target; ++i) {
            cnt_arr[i] = 0;
            sum_arr[i] = 0;
        }
        cnt_arr[0] = (seq_t)1;   /* one way to have weight 0 */
        /* unbounded coin change with weights 2,3,4, value per coin = 1 */
        const idx_t weights[3] = {2, 3, 4};
        for (int w_i = 0; w_i < 3; ++w_i) {
            idx_t w = weights[w_i];
            for (idx_t k = w; k <= target; ++k) {
                seq_t addCnt = cnt_arr[k - w];
                if (addCnt != 0) {
                    cnt_arr[k] = cnt_arr[k] + addCnt;
                    sum_arr[k] = sum_arr[k] + sum_arr[k - w] + addCnt; /* +1 per coin */
                }
            }
        }
        /* cumulative sums */
        cumC[0] = cnt_arr[0];
        cumS[0] = sum_arr[0];
        for (idx_t k = 1; k <= target; ++k) {
            cumC[k] = cumC[k - 1] + cnt_arr[k];
            cumS[k] = cumS[k - 1] + sum_arr[k];
        }
        /* clear remaining array positions up to MAX_W for safety */
        for (idx_t i = target + 1; i <= MAX_W; ++i) {
            cumC[i] = cumC[i - 1];
            cumS[i] = cumS[i - 1];
        }
    }
    /* a(n) = sum of second parts = cumS[target] + cumC[target] */
    return cumS[target] + cumC[target];
}
