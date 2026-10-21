/*
 * A296575  Number of nX5 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 0, 1 or 3 neighboring 1s.
 * https://oeis.org/A296575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Transfer‑matrix DP over rows for a 5×n binary array. */
/* Two neighbouring 1's are counted horizontally, vertically and across the
   antidiagonal.  Every 1 in the array must have 0, 1, or 3 neighbouring
   1's. The DP enumerates all 32 possible rows (masks) and precomputes
   admissible transitions. */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Helper: test whether the current row 'curr' is valid when its upper
   neighbour is 'prev' (or absent if hasPrev==false) and its lower
   neighbour is 'next' (or absent if hasNext==false).  The function only
   uses integer arithmetic. */
static bool valid_row(uint8_t prev, uint8_t curr, uint8_t next,
                      bool hasPrev, bool hasNext)
{
    for (int c = 0; c < 5; ++c) {
        if ((curr >> c) & 1u) {
            int neigh = 0;
            /* left/right in same row */
            if (c > 0 && ((curr >> (c - 1)) & 1u)) ++neigh;
            if (c < 4 && ((curr >> (c + 1)) & 1u)) ++neigh;
            /* vertical neighbours */
            if (hasPrev && ((prev >> c) & 1u)) ++neigh;
            if (hasNext && ((next >> c) & 1u)) ++neigh;
            /* antidiagonal neighbours */
            if (hasPrev && c < 4 && ((prev >> (c + 1)) & 1u)) ++neigh;
            if (hasNext && c > 0 && ((next >> (c - 1)) & 1u)) ++neigh;
            if (!(neigh == 0 || neigh == 1 || neigh == 3))
                return false;
        }
    }
    return true;
}

/* Main sequence function.  Uses static tables precomputed once. */
seq_t a(idx_t n)
{
    /* Static state */
    static bool initialized = false;
    static unsigned char nextList[32][32][32];
    static unsigned char nextCount[32][32];
    static unsigned char firstValid[32][32];
    static unsigned char lastValid[32][32];
    static unsigned char singleValid[32];

    if (!initialized) {
        /* Precompute transitions and validity tables. */
        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                /* first row (no upper neighbour) with lower neighbour = curr */
                firstValid[prev][curr] =
                    valid_row(0, (uint8_t)prev, (uint8_t)curr, false, true);

                /* last row (no lower neighbour) with upper neighbour = prev */
                lastValid[prev][curr] =
                    valid_row((uint8_t)prev, (uint8_t)curr, 0, true, false);

                /* admissible next rows for a middle row */
                unsigned int cnt = 0;
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (valid_row((uint8_t)prev, (uint8_t)curr,
                                  (uint8_t)nxt, true, true))
                        nextList[prev][curr][cnt++] = (unsigned char)nxt;
                }
                nextCount[prev][curr] = cnt;
            }
        }

        /* single row (no neighbours) */
        for (int m = 0; m < 32; ++m)
            singleValid[m] = valid_row(0, (uint8_t)m, 0, false, false);

        initialized = true;
    }

    /* Handle trivial cases */
    if (n == 0) return 0;          /* out of range, return 0 */
    if (n == 1) {
        seq_t ans = 0;
        for (int m = 0; m < 32; ++m)
            if (singleValid[m]) ans += 1;
        return ans;
    }

    /* DP tables: dp[prev][curr] holds counts for the last two rows
       processed (prev = row i‑2, curr = row i‑1). */
    static seq_t dp[32][32];
    static seq_t ndp[32][32];
    memset(dp, 0, sizeof(dp));

    /* Initialise with all valid first‑row/second‑row pairs. */
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            if (firstValid[prev][curr]) dp[prev][curr] = 1;
        }
    }

    /* Process rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        memset(ndp, 0, sizeof(ndp));
        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val) continue;
                unsigned int cnt = nextCount[prev][curr];
                unsigned char *cand = nextList[prev][curr];
                for (unsigned int k = 0; k < cnt; ++k)
                    ndp[curr][cand[k]] += val;
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Sum over states where the last row is valid. */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev)
        for (int curr = 0; curr < 32; ++curr)
            if (dp[prev][curr] && lastValid[prev][curr])
                ans += dp[prev][curr];

    return ans;
}
