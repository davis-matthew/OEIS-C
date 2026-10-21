/*
 * A298767  Number of nX5 0..1 arrays with every element equal to 1, 2, 3, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP counting n×5 binary matrices where each cell has 1,2,3,5 or 8
 *            neighbors that are equal to the cell’s value; the upper‑left
 *            cell is 0. The DP processes the matrix row by row. Top rows are
 *            validated when the next row is seen, interior rows when both
 *            above and below rows are known, and the bottom...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* number of 1..8 neighbors allowed */
static const bool allowed[9] = {
    false, /* 0 */
    true,  /* 1 */
    true,  /* 2 */
    true,  /* 3 */
    false, /* 4 */
    true,  /* 5 */
    false, /* 6 */
    false, /* 7 */
    true   /* 8 */
};

/* checks validity of a single row (no rows above or below) */
static bool valid_single(uint8_t row)
{
    for (int j = 0; j < 5; ++j) {
        int v = (row >> j) & 1;
        int cnt = 0;
        if (j > 0 && ((row >> (j - 1)) & 1) == v) ++cnt;
        if (j < 4 && ((row >> (j + 1)) & 1) == v) ++cnt;
        if (!allowed[cnt]) return false;
    }
    return true;
}

/* checks validity of the top row given the next row (no row above) */
static bool valid_top(uint8_t up, uint8_t low)
{
    for (int j = 0; j < 5; ++j) {
        int v = (up >> j) & 1;
        int cnt = 0;
        if (j > 0 && ((up >> (j - 1)) & 1) == v) ++cnt;
        if (j < 4 && ((up >> (j + 1)) & 1) == v) ++cnt;
        for (int dj = -1; dj <= 1; ++dj) {
            int nc = j + dj;
            if (0 <= nc && nc < 5) {
                int bit = (low >> nc) & 1;
                if (bit == v) ++cnt;
            }
        }
        if (!allowed[cnt]) return false;
    }
    return true;
}

/* checks validity of the bottom row given the row above (no row below) */
static bool valid_bottom(uint8_t up, uint8_t cur)
{
    for (int j = 0; j < 5; ++j) {
        int v = (cur >> j) & 1;
        int cnt = 0;
        if (j > 0 && ((cur >> (j - 1)) & 1) == v) ++cnt;
        if (j < 4 && ((cur >> (j + 1)) & 1) == v) ++cnt;
        for (int dj = -1; dj <= 1; ++dj) {
            int nc = j + dj;
            if (0 <= nc && nc < 5) {
                int bit = (up >> nc) & 1;
                if (bit == v) ++cnt;
            }
        }
        if (!allowed[cnt]) return false;
    }
    return true;
}

/* checks validity of a middle row given the rows above and below */
static bool valid_middle(uint8_t up, uint8_t mid, uint8_t low)
{
    for (int j = 0; j < 5; ++j) {
        int v = (mid >> j) & 1;
        int cnt = 0;
        /* up row */
        for (int dj = -1; dj <= 1; ++dj) {
            int nc = j + dj;
            if (0 <= nc && nc < 5) {
                int bit = (up >> nc) & 1;
                if (bit == v) ++cnt;
            }
        }
        /* mid row (excluding center) */
        if (j > 0) {
            int bit = (mid >> (j - 1)) & 1;
            if (bit == v) ++cnt;
        }
        if (j < 4) {
            int bit = (mid >> (j + 1)) & 1;
            if (bit == v) ++cnt;
        }
        /* low row */
        for (int dj = -1; dj <= 1; ++dj) {
            int nc = j + dj;
            if (0 <= nc && nc < 5) {
                int bit = (low >> nc) & 1;
                if (bit == v) ++cnt;
            }
        }
        if (!allowed[cnt]) return false;
    }
    return true;
}

/* data structures for transition lists */
typedef struct {
    uint8_t items[32];
    uint8_t cnt;
} TList;

static TList top_lists[32];          /* for rows 0..31 */
static TList mid_lists[32][32];      /* for (up,mid) pairs */

/* precompute transition lists */
static void build_transitions(void)
{
    /* top lists */
    for (int r0 = 0; r0 < 32; ++r0) {
        top_lists[r0].cnt = 0;
        for (int r1 = 0; r1 < 32; ++r1)
            if (valid_top(r0, r1))
                top_lists[r0].items[top_lists[r0].cnt++] = (uint8_t)r1;
    }
    /* middle lists */
    for (int up = 0; up < 32; ++up) {
        for (int mid = 0; mid < 32; ++mid) {
            mid_lists[up][mid].cnt = 0;
            for (int low = 0; low < 32; ++low)
                if (valid_middle(up, mid, low))
                    mid_lists[up][mid].items[mid_lists[up][mid].cnt++] = (uint8_t)low;
        }
    }
}

/* DP arrays */
static seq_t dp[32][32];
static seq_t next_dp[32][32];
/* counts for first row */
static seq_t first_row_counts[32];
/* flag whether transitions have been built */
static bool built = false;

seq_t a(idx_t n)
{
    if (built == false) {
        build_transitions();
        built = true;
    }

    /* first row patterns with left‑most bit zero */
    for (int i = 0; i < 32; ++i) first_row_counts[i] = 0;
    for (int r0 = 0; r0 < 32; ++r0)
        if ((r0 & 1) == 0)
            first_row_counts[r0] = 1;

    if (n == 1) {
        seq_t result = 0;
        for (int r0 = 0; r0 < 32; ++r0) {
            if (first_row_counts[r0] && valid_single((uint8_t)r0))
                result += first_row_counts[r0];
        }
        return result;
    }

    /* initialise DP for the second row (r = 1) */
    for (int u = 0; u < 32; ++u)
        for (int m = 0; m < 32; ++m)
            dp[u][m] = 0;

    for (int r0 = 0; r0 < 32; ++r0) {
        if (first_row_counts[r0] == 0) continue;
        seq_t c = first_row_counts[r0];
        TList *lt = &top_lists[r0];
        for (int k = 0; k < lt->cnt; ++k) {
            uint8_t r1 = lt->items[k];
            dp[r0][r1] += c;
        }
    }

    /* process rows 2 .. n-1 (indices 2..n-1) */
    for (idx_t row = 2; row < n; ++row) {
        for (int u = 0; u < 32; ++u)
            for (int m = 0; m < 32; ++m)
                next_dp[u][m] = 0;
        for (int up = 0; up < 32; ++up) {
            for (int mid = 0; mid < 32; ++mid) {
                seq_t ways = dp[up][mid];
                if (ways == 0) continue;
                TList *lt = &mid_lists[up][mid];
                for (int k = 0; k < lt->cnt; ++k) {
                    uint8_t low = lt->items[k];
                    next_dp[mid][low] += ways;
                }
            }
        }
        /* copy next to dp */
        for (int u = 0; u < 32; ++u)
            for (int m = 0; m < 32; ++m)
                dp[u][m] = next_dp[u][m];
    }

    /* final sum: validate last row */
    seq_t result = 0;
    for (int up = 0; up < 32; ++up) {
        for (int mid = 0; mid < 32; ++mid) {
            seq_t ways = dp[up][mid];
            if (ways == 0) continue;
            if (valid_bottom(up, mid))
                result += ways;
        }
    }
    return result;
}
