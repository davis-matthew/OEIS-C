/*
 * A294057  Number of set partitions of [n] such that the maximal absolute
 *          difference between consecutive elements within a block equals
 *          eight.
 * https://oeis.org/A294057
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f8(n) - f7(n) where f_k(n) is the number of set
 *            partitions of [n] such that the maximal absolute difference
 *            between consecutive elements within any block does not exceed k.
 *            For this sequence k=8 and k=7
 * Range:     u8 none, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 999 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers for types used in this file */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t count_partitions_upto(idx_t n, idx_t max_gap)
{
    /* We use a DP over the positions 1..n.  The state is an 8‑bit
       mask that records, for the next 8 positions, whether they
       already have an incoming edge from a previous element.  */

    enum { MASK_BITS = 8, MASK_SIZE = 1 << MASK_BITS };
    static seq_t dp[MASK_SIZE];
    static seq_t next_dp[MASK_SIZE];
    int i, d, mask, max_offset;

    /* Initialise */
    mem_set(dp, 0, sizeof(dp));
    dp[0] = (seq_t)1;   /* nothing selected yet */

    for (i = 1; i <= (int)n; ++i) {
        /* compute how far we can forward from position i */
        int remaining = (int)n - i;
        max_offset = (remaining > (int)max_gap ? (int)max_gap : remaining);

        mem_set(next_dp, 0, sizeof(next_dp));

        for (mask = 0; mask < MASK_SIZE; ++mask) {
            seq_t ways = dp[mask];
            if (ways == (seq_t)0) continue;

            /* Option 1: no outgoing edge from i */
            int new_mask = (mask >> 1);
            next_dp[new_mask] += ways;

            /* Options 2: outgoing edge from i to i+d (1 <= d <= max_offset) */
            for (d = 1; d <= max_offset; ++d) {
                /* Check if the candidate target is still free (no incoming yet) */
                if ((mask & (1 << d)) == 0) {
                    new_mask = (mask >> 1) | (1 << (d - 1));
                    next_dp[new_mask] += ways;
                }
            }
        }

        /* Move to next position */
        mem_copy(dp, next_dp, sizeof(dp));
    }

    /* After processing all n elements, all pending incoming bits must be zero. */
    return dp[0];
}

/* Forward declaration of helper function that counts partitions
   with maximal gap <= max_gap.  No external utilities are needed. */
static seq_t count_partitions_upto(idx_t n, idx_t max_gap);

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 9.  For smaller n we return 0. */
    if (n < 9) {
        return (seq_t)0;
    }

    seq_t f8 = count_partitions_upto(n, (idx_t)8);
    seq_t f7 = count_partitions_upto(n, (idx_t)7);

    /* f8 and f7 will always satisfy f8 >= f7 for n >= 9, so subtraction
       is safe. */
    return f8 - f7;
}

/* --------------------------------------------------------------- */
/* Count partitions of the set {1,2,…,n} where every block satisfies
   the constraint that for each block, the difference between two
   consecutive elements (in increasing order) is at most max_gap. */
