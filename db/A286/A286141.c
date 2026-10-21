/*
 * A286141  Number of partitions of n into a squarefree number of parts.
 * https://oeis.org/A286141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into a squarefree number of parts
 *            (equivalently, partitions of n whose largest part is squarefree)
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* For n = 0 the empty partition counts once. */
    if (n == 0) return (seq_t)1;

    idx_t N = n;
    /* Allocate arrays of size N+1.  calloc initializes to zero.  */
    seq_t *dp  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *res = (seq_t *)calloc(N + 1, sizeof(seq_t));

    /* dp[0] = 1  – there is one way to partition 0 with any bound.        */
    dp[0] = (seq_t)1;

    for (idx_t s = 1; s <= N; ++s) {
        /* Update dp so that dp[i] holds the number of partitions of i
           into parts ≤ s.  */
        for (idx_t i = s; i <= N; ++i) {
            dp[i] += dp[i - s];
        }

        /* If the current bound s is squarefree, add the partitions whose
           largest part is exactly s.  Those are dp[i-s] after the update
           (partitions of i-s with parts ≤ s).                                        */
        if (is_squarefree((seq_t)s)) {
            for (idx_t i = s; i <= N; ++i) {
                res[i] += dp[i - s];
            }
        }
    }

    seq_t answer = res[N];
    free(dp);
    free(res);
    return answer;
}
