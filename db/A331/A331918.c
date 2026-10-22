/*
 * A331918  Number of compositions (ordered partitions) of n into distinct odd
 *          squares.
 * https://oeis.org/A331918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   number of compositions (ordered partitions) of n into distinct
 *            odd squares
 * Range:     u8 n <= 24, u16 n <= 919, u32 n <= 2924, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

#define MAX_N 10000
#define MAX_K 51     /* maximum number of odd squares <= MAX_N (1,9,25,...,99^2) */

seq_t a(idx_t n)
{
    /* Ensure n is not beyond the preallocated array bounds */
    if (n > (seq_t)MAX_N)
        return 0;   /* N/A for larger values in this implementation */

    /* collect odd squares up to n */
    int max_k = 0;
    seq_t squares[MAX_K];
    for (idx_t i = 1; i * i <= n; i += 2) {
        squares[max_k++] = (seq_t)i * (seq_t)i;
    }

    /* DP table: dp[sum][k] = number of compositions of 'sum' using exactly 'k' squares */
    static seq_t dp[MAX_N + 1][MAX_K];
    /* clear only relevant part of the table */
    int N = (int)n;
    for (int s = 0; s <= N; ++s) {
        for (int k = 0; k <= max_k; ++k)
            dp[s][k] = 0;
    }
    dp[0][0] = (seq_t)1;

    /* process each odd square exactly once (0/1 knapsack style) */
    for (int idx = 0; idx < max_k; ++idx) {
        int w = (int)squares[idx];           /* weight of this square (fits in int) */
        for (int s = N; s >= w; --s) {       /* iterate sums in descending order */
            for (int k = 0; k < max_k; ++k) { /* new entry will be k+1 */
                seq_t val = dp[s - w][k];
                if (val) {
                    dp[s][k + 1] += val * (seq_t)(k + 1);
                }
            }
        }
    }

    /* sum over all possible numbers of squares used */
    seq_t result = 0;
    for (int k = 0; k <= max_k; ++k)
        result += dp[N][k];

    return result;
}
