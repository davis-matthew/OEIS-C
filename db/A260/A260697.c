/*
 * A260697  Number of binary words w of length n with equal numbers of 010 and
 *          101 subwords such that for every prefix of w the number of
 *          occurrences of subword 101 is larger than or equal to the number
 *          of occurrences of subword 010.
 * https://oeis.org/A260697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary words of length n with equal numbers of
 *            subwords 010 and 101 such that for every prefix the number of
 *            occurrences of 101 is larger than or equal to the number of
 *            occurrences of 010. The sequence is computed by dynamic
 *            programming over the last two bits and the current bala...
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* maximum possible balance is at most n, allocate that many entries per pair */
    idx_t max_len = n + 1;   /* +1 so that we can index up to n safely */

    seq_t *dp[4];
    seq_t *dp_next[4];
    for (int p = 0; p < 4; ++p) {
        dp[p]      = (seq_t *)calloc((size_t)max_len, sizeof(seq_t));
        dp_next[p] = (seq_t *)calloc((size_t)max_len, sizeof(seq_t));
    }

    /* initial state after two bits: balance 0 for all four possible pairs */
    for (int p = 0; p < 4; ++p) dp[p][0] = (seq_t)1;

    int maxK = 0;  /* current maximum reachable balance */

    for (idx_t len = 2; len < n; ++len) {
        int newMaxK = maxK + 1;
        /* zero dp_next up to newMaxK inclusive */
        for (int p = 0; p < 4; ++p) {
            memset(dp_next[p], 0, (size_t)(newMaxK + 1) * sizeof(seq_t));
        }

        for (int p = 0; p < 4; ++p) {
            int a_bit = (p >> 1) & 1;
            int b_bit = p & 1;
            for (int k = 0; k <= maxK; ++k) {
                seq_t cur = dp[p][k];
                if (!cur) continue;

                /* append bit 0 */
                int newk = k;
                if (a_bit == 0 && b_bit == 1) newk = k - 1;   /* pattern 010 */
                if (newk >= 0) {
                    int np = (b_bit << 1) | 0;
                    dp_next[np][newk] += cur;
                }

                /* append bit 1 */
                newk = k;
                if (a_bit == 1 && b_bit == 0) newk = k + 1;   /* pattern 101 */
                if (newk >= 0) {
                    int np = (b_bit << 1) | 1;
                    dp_next[np][newk] += cur;
                }
            }
        }

        /* swap dp and dp_next for next iteration */
        for (int p = 0; p < 4; ++p) {
            seq_t *tmp = dp[p];
            dp[p] = dp_next[p];
            dp_next[p] = tmp;
        }

        maxK = newMaxK;
    }

    /* sum over all pairs with balance 0 */
    seq_t ans = 0;
    for (int p = 0; p < 4; ++p) {
        ans += dp[p][0];
    }

    /* free allocated memory */
    for (int p = 0; p < 4; ++p) {
        free(dp[p]);
        free(dp_next[p]);
    }

    return ans;
}
