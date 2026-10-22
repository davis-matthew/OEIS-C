/*
 * A386952  Number of entries in the n-th row of Pascal's triangle not
 *          divisible by 9.
 * https://oeis.org/A386952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts k (0 ≤ k ≤ n) such that C(n,k) is not divisible by
 *            9. This is equivalent to the number of k whose binomial
 *            coefficient has exponent of 3 ≤ 1, i.e., the addition k + (n−k)
 *            in base 3 generates at most one carry. The function implements
 *            this counting by a simple DP
 * Range:     u8 n <= 322, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑3 digits (least significant first). */
    idx_t digits[64];
    size_t L = 0;
    if (n == 0) {
        digits[L++] = 0;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            digits[L++] = tmp % 3;
            tmp /= 3;
        }
    }

    /* dp[borrow_in][borrow_count] – count of ways processed so far. */
    seq_t dp[2][2] = {0};
    seq_t ndp[2][2] = {0};
    dp[0][0] = 1;

    for (size_t pos = 0; pos < L; ++pos) {
        int digit = (int)digits[pos];

        /* reset ndp */
        ndp[0][0] = ndp[0][1] = ndp[1][0] = ndp[1][1] = 0;

        for (int bi = 0; bi < 2; ++bi) {
            for (int bc = 0; bc < 2; ++bc) {
                seq_t ways = dp[bi][bc];
                if (!ways) continue;

                for (int k = 0; k <= 2; ++k) {
                    int borrow_out = (k + bi > digit) ? 1 : 0;
                    int new_bc = bc + borrow_out;
                    if (new_bc > 1) continue;
                    ndp[borrow_out][new_bc] += ways;
                }
            }
        }
        /* copy ndp into dp for next iteration */
        dp[0][0] = ndp[0][0];
        dp[0][1] = ndp[0][1];
        dp[1][0] = ndp[1][0];
        dp[1][1] = ndp[1][1];
    }

    /* Only accept paths with no leftover borrow. */
    seq_t result = dp[0][0] + dp[0][1];
    return result;
}
