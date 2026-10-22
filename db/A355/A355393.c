/*
 * A355393  Number of integer partitions of n such that, for all parts x of
 *          multiplicity 1, either x - 1 or x + 1 is also a part.
 * https://oeis.org/A355393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   computed via dynamic programming with 3 states for each part
 *            size. States: 0 – previous part size absent 1 – previous part
 *            size present and no pending requirement 2 – previous part size
 *            present as a singleton without a left neighbor Transition rules
 *            are given in the comments within the function
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    const size_t N = (size_t)n;
    const size_t tot = (N + 1) * 3;          /* sums × states */

    seq_t *dp      = alloc_seq_array(tot);
    seq_t *dp_next = alloc_seq_array(tot);

    /* zero‑initialize the arrays */
    for (size_t i = 0; i < tot; ++i) {
        dp[i]      = 0;
        dp_next[i] = 0;
    }
    /* base case: sum 0, no previous part */
    dp[0 * 3 + 0] = (seq_t)1;

    for (idx_t sz = 1; sz <= n; ++sz) {
        /* clear dp_next */
        for (size_t i = 0; i < tot; ++i) dp_next[i] = 0;

        for (idx_t sum = 0; sum <= n; ++sum) {
            size_t base = sum * 3;
            for (int st = 0; st < 3; ++st) {
                seq_t val = dp[base + st];
                if (val == 0) continue;

                /* ***** k = 0 ***** */
                if (st != 2) {              /* pending requirement cannot stay unsatisfied */
                    dp_next[base + 0] += val; /* no part at this size → state 0 */
                }

                /* ***** k = 1 ***** */
                idx_t newSum = sum + sz;
                if (newSum <= n) {
                    bool leftPresent = (st == 1 || st == 2);
                    int newState = leftPresent ? 1 : 2;
                    dp_next[newSum * 3 + newState] += val;
                }

                /* ***** k >= 2 ***** */
                idx_t maxK = (n - sum) / sz;
                if (maxK >= 2) {
                    for (idx_t k = 2; k <= maxK; ++k) {
                        newSum = sum + k * sz;
                        dp_next[newSum * 3 + 1] += val;   /* block → state 1 */
                    }
                }
            }
        }

        /* swap dp and dp_next for the next size */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    seq_t ans = dp[n * 3 + 0] + dp[n * 3 + 1];  /* final state must not be pending */
    free_seq(dp);
    free_seq(dp_next);
    return ans;
}
