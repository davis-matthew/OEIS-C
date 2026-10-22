/*
 * A301537  Number of nX4 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   check_full, check_single
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = number of n×4 binary arrays whose each cell has
    0, 1, or 3 equal neighbours (vertical or horizontal) and
    the upper‑left cell is 0.
    Computation by dynamic programming over rows with a transfer matrix.   */
#include <stdbool.h>
#include "../utils/check_full.h"
#include "../utils/check_single.h"

/* Pre‑computed validity tables */
static bool valid_init[16][16] = {0};   /* first row given second row (no up) */
static bool valid_full[16][16][16] = {0}; /* middle row given up & down */
static bool valid_last[16][16] = {0};   /* last row given previous row (no down) */
static bool valid_single[16] = {0};     /* single row (n=1) , no up/down */
static bool tables_inited = false;

/* helper: test if a middle row is valid when only down exists (top row). */
static inline bool check_top(uint8_t mid, uint8_t down) {
    for (int col = 0; col < 4; ++col) {
        int same = 0;
        uint8_t val = (mid >> col) & 1;
        if (((down >> col) & 1) == val) ++same;
        if (col > 0 && (((mid >> (col - 1)) & 1) == val)) ++same;
        if (col < 3 && (((mid >> (col + 1)) & 1) == val)) ++same;
        if (same != 0 && same != 1 && same != 3) return false;
    }
    return true;
}

/* helper: test if a middle row is valid when only up exists (bottom row). */
static inline bool check_bottom(uint8_t up, uint8_t mid) {
    for (int col = 0; col < 4; ++col) {
        int same = 0;
        uint8_t val = (mid >> col) & 1;
        if (((up >> col) & 1) == val) ++same;
        if (col > 0 && (((mid >> (col - 1)) & 1) == val)) ++same;
        if (col < 3 && (((mid >> (col + 1)) & 1) == val)) ++same;
        if (same != 0 && same != 1 && same != 3) return false;
    }
    return true;
}

/* initialise all validity tables once */
static void init_tables(void) {
    for (int up = 0; up < 16; ++up) {
        for (int mid = 0; mid < 16; ++mid) {
            for (int down = 0; down < 16; ++down) {
                valid_full[up][mid][down] = check_full((uint8_t)up,(uint8_t)mid,(uint8_t)down);
            }
            valid_init[up][mid] = check_top((uint8_t)up,(uint8_t)mid);
            valid_last[up][mid] = check_bottom((uint8_t)up,(uint8_t)mid);
            valid_single[mid] = check_single((uint8_t)mid);
        }
    }
    tables_inited = true;
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n) {
    if (!tables_inited) init_tables();

    if (n == 0) return 0;   /* not used, offset starts at 1 */

    /* n == 1 special case ----------------------------------------------- */
    if (n == 1) {
        seq_t count = 0;
        for (int mid = 0; mid < 16; ++mid) {
            if ((mid & 1) == 0 && valid_single[mid]) count += 1;
        }
        return count;
    }

    /* DP tables: dp[prev][curr] */
    static seq_t dp[16][16];
    static seq_t newdp[16][16];

    /* clear dp */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;

    /* initialize with first two rows : row0 (top) and row1 */
    for (int row0 = 0; row0 < 16; ++row0) {
        if ((row0 & 1) != 0) continue;   /* upper‑left must be 0 */
        for (int row1 = 0; row1 < 16; ++row1) {
            if (valid_init[row0][row1]) dp[row0][row1] = 1;
        }
    }

    /* process rows 2 .. n-1 (indexing from 0) */
    for (idx_t r = 2; r < n; ++r) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                newdp[i][j] = 0;

        for (int up = 0; up < 16; ++up) {
            for (int mid = 0; mid < 16; ++mid) {
                seq_t ways = dp[up][mid];
                if (ways == 0) continue;
                for (int down = 0; down < 16; ++down) {
                    if (valid_full[up][mid][down]) {
                        newdp[mid][down] += ways;
                    }
                }
            }
        }
        /* copy newdp -> dp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* finalize: last row (row n-1) must satisfy its constraints with up only */
    seq_t ans = 0;
    for (int up = 0; up < 16; ++up) {
        for (int mid = 0; mid < 16; ++mid) {
            seq_t ways = dp[up][mid];
            if (ways == 0) continue;
            if (valid_last[up][mid]) ans += ways;
        }
    }
    return ans;
}
