/*
 * A242546  Number of length n+3 0..5 arrays with no four elements in a row
 *          with pattern aabb (possibly a=b) and new values 0..5 introduced in
 *          0..5 order.
 * https://oeis.org/A242546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+3 arrays over {0..5} * with new values
 *            introduced in 0..5 order and avoiding the * contiguous pattern
 *            aabb. Computed by dynamic programming in O(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>   /* for memset */
#include "../utils/idx.h"

seq_t a(idx_t n)
{
    /* length of the arrays */
    idx_t L = n + 3;

    /* DP state: dp[k][a][b][c] flattened to array of size 7*6*6*6 = 1512.
     * k = number of distinct symbols used (1..6),
     * a,b,c = last three symbols (values 0..k-1).  */
    static seq_t dp[7 * 6 * 6 * 6];
    static seq_t next[7 * 6 * 6 * 6];

    /* initialise all states to zero for each call */
    for (int i = 0; i < 7 * 6 * 6 * 6; ++i) {
        dp[i] = 0;
        next[i] = 0;
    }

    /* start with a single element 0 */
    dp[idx(1, 0, 0, 0)] = 1;

    /* iterate over positions 2 .. L */
    for (idx_t len = 2; len <= L; ++len) {
        /* reset next array */
        for (int i = 0; i < 7 * 6 * 6 * 6; ++i)
            next[i] = 0;

        /* traverse all possible states of current length len-1 */
        for (int k = 1; k <= 6; ++k) {
            for (int a = 0; a < k; ++a) {
                for (int b = 0; b < k; ++b) {
                    for (int c = 0; c < k; ++c) {
                        seq_t val = dp[idx(k, a, b, c)];
                        if (val != 0) {
                            /* use existing symbols 0 .. k-1 */
                            for (int x = 0; x < k; ++x) {
                                if (len >= 4 && a == b && c == x)
                                    continue;   /* would form aabb */
                                next[idx(k, b, c, x)] += val;
                            }
                            /* introduce new symbol if allowed */
                            if (k < 6) {
                                int x = k;          /* new symbol */
                                /* len>=4 cannot happen with new symbol because c != x */
                                next[idx(k + 1, b, c, x)] += val;
                            }
                        }
                    }
                }
            }
        }
        /* swap dp and next for next iteration */
        for (int i = 0; i < 7 * 6 * 6 * 6; ++i) {
            dp[i] = next[i];
        }
    }

    /* sum over all final states */
    seq_t result = 0;
    for (int k = 1; k <= 6; ++k) {
        for (int a = 0; a < k; ++a) {
            for (int b = 0; b < k; ++b) {
                for (int c = 0; c < k; ++c) {
                    result += dp[idx(k, a, b, c)];
                }
            }
        }
    }
    return result;
}
