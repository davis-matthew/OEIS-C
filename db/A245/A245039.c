/*
 * A245039  Number of partitions of n where the minimal multiplicity of any
 *          part is 8.
 * https://oeis.org/A245039
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n in which the minimal *
 *            multiplicity of any part equals 8. * * Computed as (# partitions
 *            of n where every part appears at * least 8 times) minus (#
 *            partitions where every part appears at * least 9 times). The
 *            calculation is performed with dynamic * programming over...
 * Range:     u8 n <= 91, u16 n <= 191, u32 n <= 490, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 91 terms through n = 862 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------------ */
static void compute_dp(idx_t N, int m, seq_t *dp)
{
    /* dp[0] = 1; all other entries start at 0. */
    mem_set(dp, 0, (N + 1) * sizeof(seq_t));
    dp[0] = 1;

    /* Temporary array reused each iteration. */
    seq_t *use = mem_new(seq_t, N + 1);

    for (idx_t j = 1; j <= N; ++j) {
        /* Clear temporary storage. */
        mem_set(use, 0, (N + 1) * sizeof(seq_t));

        /* (1) Add the mandatory k = m copies of part 'j'. */
        idx_t shift = (idx_t)m * j;
        if (shift <= N) {
            for (idx_t t = shift; t <= N; ++t) {
                use[t] += dp[t - shift];
            }
        }

        /* (2) Allow additional copies freely.  This is an unbounded
         *     addition but only applied to states that already include
         *     the mandatory 'm' copies (represented by 'use'). */
        for (idx_t t = j; t <= N; ++t) {
            use[t] += use[t - j];
        }

        /* (3) Merge the new contributions with the previous DP state. */
        for (idx_t t = 0; t <= N; ++t) {
            dp[t] += use[t];
        }
    }

    mem_free(use);
}

/* ------------------------------------------------------------------ */
/* DP for partitions in which every part appears at least 'm' times.  */

/* ------------------------------------------------------------------ */
/* Main interface required by the tests.                               */
/* ------------------------------------------------------------------ */
static idx_t maxComputed = 0;
static seq_t *dp8 = NULL;
static seq_t *dp9 = NULL;

seq_t a(idx_t n)
{
    if (n < 8) return 0;

    /* If we haven't computed up to 'n' yet, recompute both DP tables
     * from scratch up to the new maximum index. */
    if (n > maxComputed) {
        idx_t newN = n;

        seq_t *newdp8 = mem_new(seq_t, newN + 1);
        seq_t *newdp9 = mem_new(seq_t, newN + 1);

        compute_dp(newN, 8, newdp8);
        compute_dp(newN, 9, newdp9);

        mem_free(dp8);
        mem_free(dp9);
        dp8 = newdp8;
        dp9 = newdp9;
        maxComputed = newN;
    }

    /* Result is the difference between the two DP tables. */
    return dp8[n] - dp9[n];
}
