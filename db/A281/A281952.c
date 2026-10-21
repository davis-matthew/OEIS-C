/*
 * A281952  Number of nX5 0..1 arrays with no element unequal to more than
 *          four of its king-move neighbors and with new values introduced in
 *          order 0 sequentially upwards.
 * https://oeis.org/A281952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence defined by counting n×5 binary arrays that * satisfy
 *            the following constraints: * 1) a(1) counts arrays of height 1
 *            (fixed first element 0). * 2) For every element, the number of
 *            king‑move neighbors that * differ from it does not exceed 4. *
 *            3) The first array element (top‑left) must be...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   row_is_valid_with_neighbors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/row_is_valid_with_neighbors.h"

/* Constants for the DP */
#define NONE 32                     /* index for "no previous row"          */
#define PATTERNS 32                 /* number of 5‑bit row patterns        */
#define STATE_COUNT 1056            /* (NONE+1) * PATTERNS                   */

/* Pre‑computed allowed next rows for each DP state. */
static uint8_t allowedNext[STATE_COUNT][PATTERNS];
static uint8_t allowedCount[STATE_COUNT];
static bool initialized = false;

/* Initialise tables once on first call */
static void init_tables(void)
{
    if (initialized) return;
    for (int prev = 0; prev <= NONE; ++prev) {
        for (int curr = 0; curr < PATTERNS; ++curr) {
            int id = prev * PATTERNS + curr;
            int count = 0;
            int prev_row = (prev == NONE) ? 0 : prev;
            bool havePrev = (prev != NONE);
            for (int next = 0; next < PATTERNS; ++next) {
                if (row_is_valid_with_neighbors(prev_row, curr, next, havePrev, true)) {
                    allowedNext[id][count++] = (uint8_t)next;
                }
            }
            allowedCount[id] = (uint8_t)count;
        }
    }
    initialized = true;
}

/* Main function computing the sequence. */
seq_t a(idx_t n)
{
    init_tables();

    /* n == 1 is a special case */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < PATTERNS; ++r) {
            if ((r & 1) == 0)  /* first element (bit 0) must be 0 */
                ans += (seq_t)1;
        }
        return ans;
    }

    static seq_t dp[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];
    /* initialisation: first row, no previous row */
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = (seq_t)0;
    for (int r = 0; r < PATTERNS; ++r) {
        if ((r & 1) == 0)          /* first element fixed to 0 */
            dp[NONE * PATTERNS + r] = (seq_t)1;
    }

    /* iterate for rows 2 .. n */
    for (idx_t step = 1; step <= n - 1; ++step) {
        /* zero dp_next */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = (seq_t)0;

        for (int id = 0; id < STATE_COUNT; ++id) {
            seq_t ways = dp[id];
            if (ways == (seq_t)0) continue;
            int curr = id % PATTERNS;
            int count = allowedCount[id];
            for (int k = 0; k < count; ++k) {
                int next = allowedNext[id][k];
                int newid = curr * PATTERNS + next;
                dp_next[newid] += ways;
            }
        }

        /* swap dp and dp_next */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = dp_next[i];
    }

    /* sum over last row satisfying constraints with no lower neighbour */
    seq_t ans = 0;
    for (int id = 0; id < STATE_COUNT; ++id) {
        seq_t ways = dp[id];
        if (ways == (seq_t)0) continue;
        int prev = id / PATTERNS;
        int curr = id % PATTERNS;
        int prev_row = (prev == NONE) ? 0 : prev;
        if (row_is_valid_with_neighbors(prev_row, curr, 0, prev != NONE, false))
            ans += ways;
    }
    return ans;
}
