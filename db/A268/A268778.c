/*
 * A268778  Number of nX5 binary arrays with some element plus some
 *          horizontally, vertically, diagonally or antidiagonally adjacent
 *          neighbor totalling two no more than once.
 * https://oeis.org/A268778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268778: number of n×5 binary arrays with at most one pair
 * of 1s adjacent in any of the eight neighbour directions
 * (horizontal, vertical, diagonal or anti‑diagonal).  */
#include <stdint.h>
#include <stdbool.h>

/* The function satisfies the required external linkage:
 *   seq_t a(idx_t n)
 * where seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* constants for a 5‑column array */
    enum { H     = 5 };
    enum { MASKS = 1U << H };   /* all 5‑bit masks (0 … 31) */

    /* DP tables: dp[mask][flag]  flag = 0 if no adjacency yet,
     * flag = 1 if exactly one adjacency has occurred. */
    static seq_t dp[MASKS][2];
    static seq_t ndp[MASKS][2];

    /* Helper tables: horizontal adjacency in a single row,
     * edges between two consecutive rows. */
    static unsigned char horiz[MASKS];
    static unsigned char between[MASKS][MASKS];
    static bool initialized = false;

    /* Pre‑compute helper tables once. */
    if (!initialized) {
        /* horizontal edges within a single mask */
        for (unsigned mask = 0; mask < MASKS; ++mask) {
            unsigned cnt = 0;
            for (unsigned r = 0; r + 1 < H; ++r)
                if ( ((mask >> r) & 1U) && ((mask >> (r + 1)) & 1U) )
                    ++cnt;
            horiz[mask] = (unsigned char)cnt;
        }

        /* edges between two consecutive rows */
        for (unsigned prev = 0; prev < MASKS; ++prev) {
            for (unsigned cur = 0; cur < MASKS; ++cur) {
                unsigned cnt = 0;
                /* vertical */
                for (unsigned r = 0; r < H; ++r)
                    if ( ((prev >> r) & 1U) && ((cur >> r) & 1U) )
                        ++cnt;
                /* diagonal right (prev r ↔ cur r+1) */
                for (unsigned r = 0; r + 1 < H; ++r)
                    if ( ((prev >> r) & 1U) && ((cur >> (r + 1)) & 1U) )
                        ++cnt;
                /* diagonal left  (prev r ↔ cur r-1) */
                for (unsigned r = 1; r < H; ++r)
                    if ( ((prev >> r) & 1U) && ((cur >> (r - 1)) & 1U) )
                        ++cnt;
                between[prev][cur] = (unsigned char)cnt;
            }
        }
        initialized = true;
    }

    /* initialise first row (previous row considered all zeros) */
    for (unsigned m = 0; m < MASKS; ++m) {
        dp[m][0] = 0;
        dp[m][1] = 0;
    }
    dp[0][0] = 1;  /* start with an empty previous row, no adjacency used */

    /* DP over rows */
    for (idx_t row = 0; row < n; ++row) {
        /* clear next table */
        for (unsigned m = 0; m < MASKS; ++m) {
            ndp[m][0] = 0;
            ndp[m][1] = 0;
        }

        /* transition from every previous mask */
        for (unsigned prev = 0; prev < MASKS; ++prev) {
            for (unsigned flag = 0; flag < 2; ++flag) {
                seq_t ways = dp[prev][flag];
                if (!ways) continue;

                for (unsigned cur = 0; cur < MASKS; ++cur) {
                    /* edges added by this new row */
                    unsigned added = between[prev][cur] + horiz[cur];
                    unsigned new_flag = flag + added;
                    if (new_flag > 1) continue;        /* more than one adjacency */
                    ndp[cur][new_flag] += ways;
                }
            }
        }

        /* move to the next row */
        for (unsigned m = 0; m < MASKS; ++m) {
            dp[m][0] = ndp[m][0];
            dp[m][1] = ndp[m][1];
        }
    }

    /* sum all final states */
    seq_t result = 0;
    for (unsigned m = 0; m < MASKS; ++m) {
        result += dp[m][0];
        result += dp[m][1];
    }
    return result;
}
