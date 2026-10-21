/*
 * A269077  Number of 3 X n binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   conflict
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269077: Number of 3×n binary arrays with at most one pair of
   adjacent 1s (horizontally, diagonally, or anti‑diagonally). */
/* The DP keeps, for each column, the number of ways to have seen 0 or 1
   adjacent pairs so far.  Transitions between two adjacent columns
   add 0, 1, or 2 conflicts; we discard transitions that would give
   more than one conflict in total. */

#include <stdbool.h>
#include "../utils/conflict.h"

/* External linkage: seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not used: sequence starts at n=1 */

    if (n == 1)
        return (seq_t)8;          /* all 3×1 binary arrays are allowed */

    /* DP arrays: prev[s][k] -- number of ways to reach column with
       pattern s and k conflicts (k = 0 or 1). */
    seq_t prev[8][2];
    seq_t curr[8][2];

    /* initialise for column 0 (no conflicts yet) */
    for (int s = 0; s < 8; ++s)
    {
        prev[s][0] = (seq_t)1;
        prev[s][1] = (seq_t)0;
    }

    /* iterate over remaining columns */
    for (idx_t col = 1; col < n; ++col)
    {
        /* zero current DP */
        for (int s = 0; s < 8; ++s)
        {
            curr[s][0] = (seq_t)0;
            curr[s][1] = (seq_t)0;
        }

        /* transition from previous column to current */
        for (int sprev = 0; sprev < 8; ++sprev)
        {
            seq_t val0 = prev[sprev][0];
            seq_t val1 = prev[sprev][1];

            if ((val0 == (seq_t)0) && (val1 == (seq_t)0))
                continue;

            for (int scurr = 0; scurr < 8; ++scurr)
            {
                int conf = conflict(sprev, scurr);
                if (conf >= 2)
                    continue;

                if (conf == 0)
                {
                    curr[scurr][0] += val0;
                    curr[scurr][1] += val1;
                }
                else /* conf == 1 */
                {
                    curr[scurr][1] += val0;
                }
            }
        }

        /* copy curr to prev for next iteration */
        for (int s = 0; s < 8; ++s)
        {
            prev[s][0] = curr[s][0];
            prev[s][1] = curr[s][1];
        }
    }

    /* sum all ways with 0 or 1 conflict after the last column */
    seq_t result = (seq_t)0;
    for (int s = 0; s < 8; ++s)
    {
        result += prev[s][0];
        result += prev[s][1];
    }

    return result;
}
