/*
 * A296736  Number of n X 5 0..1 arrays with each 1 adjacent to 1, 2 or 4
 *          king-move neighboring 1's.
 * https://oeis.org/A296736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   neighbor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute a(n) = number of n×5 binary arrays such that every 1 has
 * exactly 1, 2 or 4 king‑move neighboring 1's.
 * 
 * The algorithm uses dynamic programming over two consecutive rows.
 * State: (row i‑1, row i).  Transition: add new row (row i+1);
 * the row i must be checked now, while row i+1 is pending.
 * After n rows the last row is checked with no below neighbors.
 * The transition table is pre‑computed once.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/neighbor_count.h"

static uint32_t nextMask[32][32];
static bool lastValid[32][32];
static bool table_prepared = false;

/* Check if all 1’s in curr are valid when adjacent rows are prev and next. */
static bool valid_transition(int prev, int curr, int next)
{
    for (int j = 0; j < 5; ++j) {
        if ((curr >> j) & 1) {
            int c = neighbor_count(prev, curr, next, j);
            if (!(c == 1 || c == 2 || c == 4)) return false;
        }
    }
    return true;
}

/* Check if all 1’s in curr are valid when there is no row below (next = 0). */
static bool valid_last_row(int prev, int curr)
{
    return valid_transition(prev, curr, 0);
}

/* Prepare tables only once. */
static void prepare_tables(void)
{
    if (table_prepared) return;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            uint32_t mask = 0;
            for (int next = 0; next < 32; ++next) {
                if (valid_transition(prev, curr, next))
                    mask |= 1U << next;
            }
            nextMask[prev][curr] = mask;
            lastValid[prev][curr] = valid_last_row(prev, curr);
        }
    }
    table_prepared = true;
}

/* Main function as required. */
seq_t a(idx_t n)
{
    prepare_tables();

    /* DP arrays: dp[prev][curr] and newdp[curr][next] */
    seq_t dp[32][32];
    seq_t newdp[32][32];
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j) dp[i][j] = 0;
    dp[0][0] = 1;             /* start with no rows */

    for (idx_t row = 1; row <= n; ++row) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j) newdp[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                if (dp[prev][curr] == 0) continue;
                uint32_t mask = nextMask[prev][curr];
                /* iterate over set bits of mask */
                while (mask) {
                    unsigned next = __builtin_ctz(mask);
                    mask &= mask - 1;
                    newdp[curr][next] += dp[prev][curr];
                }
            }
        }
        /* copy newdp to dp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j) dp[i][j] = newdp[i][j];
    }

    /* Sum over states with last row valid. */
    seq_t result = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            if (dp[prev][curr] == 0) continue;
            if (lastValid[prev][curr]) result += dp[prev][curr];
        }
    }
    return result;
}
