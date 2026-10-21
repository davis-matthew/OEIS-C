/*
 * A282335  Number of nX5 0..1 arrays with no 1 equal to more than four of its
 *          king-move neighbors, with the exception of exactly one element.
 * https://oeis.org/A282335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   computeDelta
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count  n×5 binary arrays that have exactly one cell with more than four
   king–move neighbours equal to 1.  The DP keeps track of the two
   previous rows and the number of violating cells seen so far
   (0 or 1).  The first two rows are initialized with the upper
   neighbourhood being all zero.  For each additional row we add the
   current row as the new lower neighbour and update the count of
   violations for the middle row.  Upon reaching the last row we
   evaluate the final row (whose lower neighbours are all zero). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/computeDelta.h"

static unsigned char preDelta[32][32][32];
static bool preDelta_initialized = false;

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* One‑time initialization of preDelta. */
    if (!preDelta_initialized) {
        for (unsigned int up = 0; up < 32; ++up)
            for (unsigned int mid = 0; mid < 32; ++mid)
                for (unsigned int down = 0; down < 32; ++down)
                    preDelta[up][mid][down] = computeDelta(up, mid, down);
        preDelta_initialized = true;
    }

    /* DP tables.  dp[up][mid][viol] holds the count for the last two rows
       'up' and 'mid' with 'viol' violations so far. */
    static seq_t dp[32][32][2];
    static seq_t newdp[32][32][2];

    /* Zero dp and newdp.  Use loops instead of memset to avoid
       corrupting non‑trivial types in the arbitrary‑precision case. */
    for (unsigned int u = 0; u < 32; ++u)
        for (unsigned int m = 0; m < 32; ++m)
            for (unsigned int v = 0; v < 2; ++v) {
                dp[u][m][v] = 0;
                newdp[u][m][v] = 0;
            }

    /* Initialise with the first two rows: upper neighbours are all zero. */
    for (unsigned int r0 = 0; r0 < 32; ++r0)
        for (unsigned int r1 = 0; r1 < 32; ++r1) {
            unsigned char delta0 = preDelta[0][r0][r1];
            if (delta0 <= 1)
                dp[r0][r1][delta0] += (seq_t)1;
        }

    /* Process rows 2 .. n-1 (0‑based indexing). */
    for (unsigned int idx = 2; idx < (unsigned int)n; ++idx) {
        /* Zero newdp. */
        for (unsigned int u = 0; u < 32; ++u)
            for (unsigned int m = 0; m < 32; ++m)
                for (unsigned int v = 0; v < 2; ++v)
                    newdp[u][m][v] = 0;

        for (unsigned int up = 0; up < 32; ++up)
            for (unsigned int mid = 0; mid < 32; ++mid)
                for (unsigned int viol = 0; viol < 2; ++viol) {
                    seq_t cur = dp[up][mid][viol];
                    if (cur == 0) continue;
                    for (unsigned int down = 0; down < 32; ++down) {
                        unsigned char delta = preDelta[up][mid][down];
                        unsigned int newViol = viol + delta;
                        if (newViol > 1) continue;
                        newdp[mid][down][newViol] += cur;
                    }
                }

        /* Copy newdp to dp for next iteration. */
        for (unsigned int u = 0; u < 32; ++u)
            for (unsigned int m = 0; m < 32; ++m)
                for (unsigned int v = 0; v < 2; ++v)
                    dp[u][m][v] = newdp[u][m][v];
    }

    /* Evaluate the last row (lower neighbours all zero). */
    seq_t result = 0;
    for (unsigned int up = 0; up < 32; ++up)
        for (unsigned int mid = 0; mid < 32; ++mid) {
            unsigned char deltaLast = preDelta[up][mid][0];
            for (unsigned int viol = 0; viol < 2; ++viol) {
                seq_t cur = dp[up][mid][viol];
                if (cur == 0) continue;
                unsigned int finalViol = viol + deltaLast;
                if (finalViol == 1)
                    result += cur;
            }
        }

    return result;
}
