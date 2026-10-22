/*
 * A383549  Number of rises in all compositions of n with parts in {1,2,3} and
 *          adjacent differences in {-1,1}.
 * https://oeis.org/A383549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 48, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * OEIS A383549: Number of rises in all compositions of n with parts in {1,2,3} 
 * and adjacent differences in {-1,1}.  
 * The sequence is computed by a simple linear DP over the last part (1,2,3).
 *      dp1[n] – number of compositions of n ending with 1
 *      dp2[n] – number of compositions of n ending with 2
 *      dp3[n] – number of compositions of n ending with 3
 *      sum1[n] – total rises over all such compositions ending with 1
 *      sum2[n] – total rises over all such compositions ending with 2
 *      sum3[n] – total rises over all such compositions ending with 3
 * Recurrence (for n ≥ 4):
 *      dp1[n] = dp2[n‑1]
 *      dp2[n] = dp1[n‑2] + dp3[n‑2]
 *      dp3[n] = dp2[n‑3]
 *
 *      sum1[n] = sum2[n‑1]
 *      sum2[n] = sum1[n‑2] + dp1[n‑2] + sum3[n‑2]
 *      sum3[n] = sum2[n‑3] + dp2[n‑3]
 * Base values:
 *      dp1[1] = 1,  sum1[1] = 0
 *      dp2[2] = 1,  sum2[2] = 0
 *      dp3[3] = 1,  sum3[3] = 0
 * The answer a(n) is sum1[n] + sum2[n] + sum3[n].
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Allocate dynamic arrays – alloc_seq_array safely constructs objects. */
    seq_t *dp1 = alloc_seq_array((size_t)(n + 1));
    seq_t *dp2 = alloc_seq_array((size_t)(n + 1));
    seq_t *dp3 = alloc_seq_array((size_t)(n + 1));
    seq_t *sum1 = alloc_seq_array((size_t)(n + 1));
    seq_t *sum2 = alloc_seq_array((size_t)(n + 1));
    seq_t *sum3 = alloc_seq_array((size_t)(n + 1));

    if (!dp1 || !dp2 || !dp3 || !sum1 || !sum2 || !sum3)
    {
        /* Allocation failure – return 0 (undefined behaviour otherwise). */
        free_seq(dp1); free_seq(dp2); free_seq(dp3);
        free_seq(sum1); free_seq(sum2); free_seq(sum3);
        return (seq_t)0;
    }

    /* Zero the arrays (alloc_seq_array might not zero it). */
    if (dp1) { for (idx_t i = 0; i <= n; ++i) dp1[i] = (seq_t)0; }
    if (dp2) { for (idx_t i = 0; i <= n; ++i) dp2[i] = (seq_t)0; }
    if (dp3) { for (idx_t i = 0; i <= n; ++i) dp3[i] = (seq_t)0; }
    if (sum1) { for (idx_t i = 0; i <= n; ++i) sum1[i] = (seq_t)0; }
    if (sum2) { for (idx_t i = 0; i <= n; ++i) sum2[i] = (seq_t)0; }
    if (sum3) { for (idx_t i = 0; i <= n; ++i) sum3[i] = (seq_t)0; }

    /* Base cases. */
    if (n >= 1) { dp1[1] = (seq_t)1; sum1[1] = (seq_t)0; }
    if (n >= 2) { dp2[2] = (seq_t)1; sum2[2] = (seq_t)0; }
    if (n >= 3) { dp3[3] = (seq_t)1; sum3[3] = (seq_t)0; }

    /* DP for n >= 2.  We skip n==1 as already handled. */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (i == 2) {
            /* dp1[2] stays 0, sum1[2] already 0, dp2[2] and sum2[2] set above. */
            continue;
        }
        if (i == 3) {
            /* Compute values that depend on already known base entries. */
            dp1[3] = dp2[2];
            sum1[3] = sum2[2];

            dp2[3] = dp1[1] + dp3[1];
            sum2[3] = sum1[1] + dp1[1] + sum3[1];

            /* dp3[3] and sum3[3] are already set by the base case. */
            continue;
        }

        /* i >= 4 */
        dp1[i] = dp2[i - 1];
        sum1[i] = sum2[i - 1];

        dp2[i] = dp1[i - 2] + dp3[i - 2];
        sum2[i] = sum1[i - 2] + dp1[i - 2] + sum3[i - 2];

        dp3[i] = dp2[i - 3];
        sum3[i] = sum2[i - 3] + dp2[i - 3];
    }

    seq_t result = sum1[n] + sum2[n] + sum3[n];

    /* Clean up. */
    free_seq(dp1); free_seq(dp2); free_seq(dp3);
    free_seq(sum1); free_seq(sum2); free_seq(sum3);

    return result;
}
