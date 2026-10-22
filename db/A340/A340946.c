/*
 * A340946  Number of ways to write n as an ordered sum of 9 squares of
 *          positive integers.
 * https://oeis.org/A340946
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 9‑tuples of positive integers whose
 *            squares sum to n
 * Range:     u8 n <= 25, u16 n <= 88, u32 n <= 1632, u64 n <= 9412,
 *            u128 n <= 9412, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 9U) return 0;          /* offset is 9, behavior below is irrelevant */

    static idx_t max_n = 0;
    static seq_t *dp = NULL;
    static seq_t *next_arr = NULL;

    if (n > max_n) {
        idx_t size = n + 1;

#ifdef __cplusplus
        delete[] dp;
        delete[] next_arr;
        dp = new seq_t[size]();          /* value‑initialize all elements to zero */
        next_arr = new seq_t[size]();
#else
        free(dp);
        free(next_arr);
        dp = (seq_t *)calloc((size_t)size, sizeof(seq_t));
        next_arr = (seq_t *)calloc((size_t)size, sizeof(seq_t));
#endif

        /* list of square values up to n */
        idx_t squares[101];
        size_t sq_cnt = 0;
        for (idx_t k = 1; k * k <= (idx_t)n; ++k) {
            squares[sq_cnt++] = k * k;
        }

        /* initial state: 0 squares used, sum 0 */
        for (idx_t i = 0; i <= (idx_t)n; ++i) dp[i] = 0;
        dp[0] = 1;

        /* perform 9 convolutions */
        for (int step = 0; step < 9; ++step) {
            for (idx_t i = 0; i <= (idx_t)n; ++i) next_arr[i] = 0;

            for (idx_t s = 0; s <= (idx_t)n; ++s) {
                seq_t cur = dp[s];
                if (cur == 0) continue;

                for (size_t si = 0; si < sq_cnt; ++si) {
                    idx_t ns = s + squares[si];
                    if (ns > (idx_t)n) break;           /* squares sorted */
                    next_arr[ns] += cur;
                }
            }
            /* swap dp and next_arr for next iteration */
            seq_t *tmp = dp;
            dp = next_arr;
            next_arr = tmp;
        }

        max_n = n;
    }

    return dp[n];
}
