/*
 * A298962  Number of nX7 0..1 arrays with every element equal to 3, 5, 6, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×7 binary arrays such that every cell has 3, 5, 6, 7,
 *            or 8 king‑neighbour cells equal to itself, with the upper left
 *            element fixed to 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses a transfer matrix over pairs of rows.
   Precomputation of valid triples: (a,b,c) where row b satisfies
   its constraint with rows a (above) and c (below). */
/* Complexity: O(n) with a small constant (≈128 × 128 × avg_next). */

#include <stdbool.h>
#include <stdint.h>

/* Forward declarations for seq_t and idx_t are provided by the build environment. */

/* Helper: test whether a row satisfies its constraint
   GIVEN the existence (or lack) of an above and/or below row. */
static bool valid_row(uint8_t row, bool above_exists, uint8_t above,
                      bool below_exists, uint8_t below)
{
    /* Allowed neighbour counts */
    static const bool allowed[9] = {
        false, false, false, true, false,
        true, true, true, true
    };

    for (int j = 0; j < 7; ++j) {
        int same = (row >> j) & 1;
        int cnt = 0;

        /* same row neighbours */
        if (j > 0   && same == ((row >> (j - 1)) & 1)) ++cnt;
        if (j + 1 < 7 && same == ((row >> (j + 1)) & 1)) ++cnt;

        /* above row neighbours */
        if (above_exists) {
            if (j > 0   && same == ((above >> (j - 1)) & 1)) ++cnt;
            if (same == ((above >> j) & 1))                 ++cnt;
            if (j + 1 < 7 && same == ((above >> (j + 1)) & 1)) ++cnt;
        }

        /* below row neighbours */
        if (below_exists) {
            if (j > 0   && same == ((below >> (j - 1)) & 1)) ++cnt;
            if (same == ((below >> j) & 1))                 ++cnt;
            if (j + 1 < 7 && same == ((below >> (j + 1)) & 1)) ++cnt;
        }

        if (!allowed[cnt]) return false;
    }
    return true;
}

/* Structure to hold lists of admissible next rows for each (prevprev,prev) pair. */
struct NextList {
    unsigned char count;            /* how many admissible next rows */
    uint8_t next[128];              /* indices of admissible next rows   */
};

static struct NextList lists[128][128];
static bool precomp_done = false;

/* Precompute valid triples and the corresponding lists. */
static void precompute(void)
{
    if (precomp_done) return;
    precomp_done = true;

    for (int a = 0; a < 128; ++a) {
        for (int b = 0; b < 128; ++b) {
            struct NextList *lst = &lists[a][b];
            lst->count = 0;
            for (int c = 0; c < 128; ++c) {
                if (valid_row(b, true /*above_exists*/ , (uint8_t)a,
                               true /*below_exists*/ , (uint8_t)c)) {
                    /* store admissible next row */
                    lst->next[lst->count++] = (uint8_t)c;
                }
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* below the offset the value is irrelevant */

    precompute();

    /* Special cases n = 1 and n = 2 */
    if (n == 1) return 0;

    /* dp arrays: dp[prevprev][prev] */
    static seq_t dp[128][128];
    static seq_t newdp[128][128];
    seq_t result = 0;

    if (n == 2) {
        /* Count all pairs (row1,row2) satisfying top and bottom constraints */
        for (int row1 = 0; row1 < 128; ++row1) {
            if (row1 & 1) continue;                /* upper left element must be 0 */
            for (int row2 = 0; row2 < 128; ++row2) {
                if (!valid_row((uint8_t)row1, false, 0, true, (uint8_t)row2)) continue; /* top */
                if (!valid_row((uint8_t)row2, true, (uint8_t)row1, false, 0)) continue; /* bottom */
                result += 1;
            }
        }
        return result;
    }

    /* n >= 3 */

    /* Initialise dp with pairs (row1,row2) that satisfy the top constraint. */
    for (int i = 0; i < 128; ++i)
        for (int j = 0; j < 128; ++j)
            dp[i][j] = 0;

    for (int row1 = 0; row1 < 128; ++row1) {
        if (row1 & 1) continue;   /* upper left cell must be 0 */
        for (int row2 = 0; row2 < 128; ++row2) {
            if (!valid_row((uint8_t)row1, false, 0, true, (uint8_t)row2)) continue;
            dp[row1][row2] += 1;
        }
    }

    /* Iterate through rows 3..n-1, building the DP. */
    for (idx_t step = 3; step <= n; ++step) {
        /* Zero newdp */
        for (int i = 0; i < 128; ++i)
            for (int j = 0; j < 128; ++j)
                newdp[i][j] = 0;

        for (int a = 0; a < 128; ++a) {
            for (int b = 0; b < 128; ++b) {
                if (dp[a][b] == 0) continue;
                const struct NextList *lst = &lists[a][b];
                for (unsigned char k = 0; k < lst->count; ++k) {
                    uint8_t c = lst->next[k];
                    newdp[b][c] += dp[a][b];
                }
            }
        }

        /* swap dp and newdp */
        for (int i = 0; i < 128; ++i)
            for (int j = 0; j < 128; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* After all rows processed, dp holds states (row_{n-1}, row_n).
       We must ensure the last row satisfies its bottom constraint. */
    result = 0;
    for (int a = 0; a < 128; ++a) {
        for (int b = 0; b < 128; ++b) {
            if (dp[a][b] == 0) continue;
            if (valid_row((uint8_t)b, true, (uint8_t)a, false, 0)) {
                result += dp[a][b];
            }
        }
    }

    return result;
}
