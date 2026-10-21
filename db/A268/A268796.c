/*
 * A268796  Number of nX6 0..2 arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A268796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Number of n×6 arrays with entries 0..2 that contain 
 * exactly one pair of adjacent cells (horizontally, vertically, or 
 * antidiagonally) whose values sum to 2.
 * 
 * Computation: dynamic programming over rows (state 0 or 1 accumulated pairs).
 * 
 * MF: horizontal pairs are counted within each row, vertical and antidiagonal
 * (down‑left) pairs are counted between successive rows.  See the code for
 * implementation details.
 */

#include <stdint.h>
#include <stdbool.h>

#define NUM_ROWS 729            /* 3^6 possible rows */
#define MAX_NEIGH 1000          /* safety margin for adjacency lists */

static uint8_t digits[NUM_ROWS][6];      /* trit digits of each row */
static uint8_t withinRow[NUM_ROWS];      /* horizontal pairs sum to 2 inside a row */

/* adjacency lists for a row r1 → r2 that contribute 0 or 1 total pairs
 * (cross = vertical + anti­diagonal (down‑left)). */
static uint16_t adj00cnt[NUM_ROWS],   /* cross=0, within=0 → new count 0   */
                adj01cnt[NUM_ROWS],   /* cross=1, within=0 → new count 1   */
                adj10cnt[NUM_ROWS];   /* cross=0, within=1 → new count 1   */

static uint16_t adj00[NUM_ROWS][MAX_NEIGH],
                adj01[NUM_ROWS][MAX_NEIGH],
                adj10[NUM_ROWS][MAX_NEIGH];

static bool tables_inited = false;

/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    if (tables_inited) return;
    tables_inited = true;

    /* compute trit digits and horizontal pair counts for each row */
    for (int r = 0; r < NUM_ROWS; ++r) {
        int v = r;
        for (int c = 0; c < 6; ++c) {
            digits[r][c] = (uint8_t)(v % 3);
            v /= 3;
        }
        uint8_t w = 0;
        for (int c = 0; c < 5; ++c)
            if (digits[r][c] + digits[r][c + 1] == 2) ++w;
        withinRow[r] = w;
    }

    /* build adjacency lists between successive rows, counting only
     * vertical and anti‑diagonal (down‑left) pairs. */
    for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
        adj00cnt[r1] = adj01cnt[r1] = adj10cnt[r1] = 0;
        for (int r2 = 0; r2 < NUM_ROWS; ++r2) {
            uint8_t cross = 0;

            /* vertical pairs */
            for (int c = 0; c < 6; ++c)
                if (digits[r1][c] + digits[r2][c] == 2) ++cross;

            /* anti‑diagonal (down‑left) pairs: (r1,c) with (r2,c‑1) */
            for (int c = 1; c < 6; ++c)
                if (digits[r1][c] + digits[r2][c - 1] == 2) ++cross;

            /* only cross 0 or 1 can lead to a valid final count of 1 */
            if (cross > 1) continue;

            uint8_t w = withinRow[r2];
            if (cross == 0) {
                if (w == 0) {
                    adj00[r1][adj00cnt[r1]++] = (uint16_t)r2;
                } else if (w == 1) {
                    adj10[r1][adj10cnt[r1]++] = (uint16_t)r2;
                }
            } else { /* cross == 1 */
                if (w == 0) {
                    adj01[r1][adj01cnt[r1]++] = (uint16_t)r2;
                }
                /* cross==1 with w==1 produces 2 pairs – not allowed, skip */
            }
        }
    }
}
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init_tables();

    /* dp arrays: dp0[r] = ways with current row r having accumulated 0 pairs,
                     dp1[r] = ways with accumulated 1 pair.  */
    static seq_t dp0[NUM_ROWS], dp1[NUM_ROWS];
    static seq_t dp0_next[NUM_ROWS], dp1_next[NUM_ROWS];

    /* initialise first row */
    for (int r = 0; r < NUM_ROWS; ++r) {
        dp0[r] = 0;
        dp1[r] = 0;
    }
    for (int r = 0; r < NUM_ROWS; ++r) {
        if (withinRow[r] == 0) dp0[r] = (seq_t)1;
        else if (withinRow[r] == 1) dp1[r] = (seq_t)1;
    }

    if (n == 1) {
        seq_t res = 0;
        for (int r = 0; r < NUM_ROWS; ++r) res += dp1[r];
        return res;
    }

    /* iterate over remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero next arrays */
        for (int r = 0; r < NUM_ROWS; ++r) {
            dp0_next[r] = 0;
            dp1_next[r] = 0;
        }

        for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
            seq_t val0 = dp0[r1];
            seq_t val1 = dp1[r1];

            if (val0) {
                /* cross==0, within==0 → stay at 0 */
                for (int k = 0; k < adj00cnt[r1]; ++k)
                    dp0_next[adj00[r1][k]] += val0;

                /* cross==1, within==0 → become 1 */
                for (int k = 0; k < adj01cnt[r1]; ++k)
                    dp1_next[adj01[r1][k]] += val0;

                /* cross==0, within==1 → become 1 */
                for (int k = 0; k < adj10cnt[r1]; ++k)
                    dp1_next[adj10[r1][k]] += val0;
            }

            if (val1) {
                /* only cross==0, within==0 keeps count at 1 */
                for (int k = 0; k < adj00cnt[r1]; ++k)
                    dp1_next[adj00[r1][k]] += val1;
            }
        }

        /* copy next to current for next iteration */
        for (int r = 0; r < NUM_ROWS; ++r) {
            dp0[r] = dp0_next[r];
            dp1[r] = dp1_next[r];
        }
    }

    /* total arrays with exactly one adjacent pair */
    seq_t result = 0;
    for (int r = 0; r < NUM_ROWS; ++r) result += dp1[r];
    return result;
}
