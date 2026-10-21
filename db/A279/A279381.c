/*
 * A279381  Number of nX5 0..1 arrays with no element equal to a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors,
 *          with new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A279381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of n×5 0/1 arrays with the strict majority
 *            condition) / 2 The division by 2 is needed because the OEIS
 *            sequence counts arrays up to complementary (bitwise NOT)
 *            symmetry
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define W 5            /* width */
#define STATES (1 << W)/* 32 */

/* validity tables */
static bool first_valid[STATES][STATES] = {0};
static bool last_valid[STATES][STATES]  = {0};
static bool mid_valid[STATES][STATES][STATES] = {0};
static bool one_row_valid[STATES] = {0};
static bool tables_prepared = false;

/* helper to get bit of a row */
static inline int bit(int row, int pos) {
    return (row >> pos) & 1;
}

/* check a single row (n==1) */
static bool good_one_row(int row) {
    for (int c = 0; c < W; ++c) {
        int val = bit(row, c);
        int k = 0, same = 0;
        if (c > 0)  { k++; if (bit(row, c-1) == val) same++; }
        if (c < W-1){ k++; if (bit(row, c+1) == val) same++; }
        if (same * 2 > k) return false;
    }
    return true;
}

/* first row (only up to second row below) */
static bool good_first(int cur, int nxt) {
    for (int c = 0; c < W; ++c) {
        int val = bit(cur, c);
        int k = 0, same = 0;
        if (c > 0)  { k++; if (bit(cur, c-1) == val) same++; }
        if (c < W-1){ k++; if (bit(cur, c+1) == val) same++; }
        /* down */
        k++; if (bit(nxt, c) == val) same++;
        /* SW */
        if (c > 0)  { k++; if (bit(nxt, c-1) == val) same++; }
        if (same * 2 > k) return false;
    }
    return true;
}

/* last row (only up/NE) */
static bool good_last(int prev, int cur) {
    for (int c = 0; c < W; ++c) {
        int val = bit(cur, c);
        int k = 0, same = 0;
        if (c > 0)  { k++; if (bit(cur, c-1) == val) same++; }
        if (c < W-1){ k++; if (bit(cur, c+1) == val) same++; }
        /* up */
        k++; if (bit(prev, c) == val) same++;
        /* NE */
        if (c < W-1){ k++; if (bit(prev, c+1) == val) same++; }
        if (same * 2 > k) return false;
    }
    return true;
}

/* middle row given up, cur, down */
static bool good_middle(int up, int cur, int down) {
    for (int c = 0; c < W; ++c) {
        int val = bit(cur, c);
        int k = 0, same = 0;
        if (c > 0)  { k++; if (bit(cur, c-1) == val) same++; }
        if (c < W-1){ k++; if (bit(cur, c+1) == val) same++; }
        /* up */
        k++; if (bit(up, c) == val) same++;
        /* down */
        k++; if (bit(down, c) == val) same++;
        /* NE */
        if (c < W-1){ k++; if (bit(up, c+1) == val) same++; }
        /* SW */
        if (c > 0)  { k++; if (bit(down, c-1) == val) same++; }
        if (same * 2 > k) return false;
    }
    return true;
}

/* build validity tables once */
static void prepare_tables(void) {
    if (tables_prepared) return;
    for (int r = 0; r < STATES; ++r) {
        one_row_valid[r] = good_one_row(r);
        for (int s = 0; s < STATES; ++s) {
            first_valid[r][s] = good_first(r, s);
            last_valid[r][s]  = good_last(r, s);
        }
    }
    for (int u = 0; u < STATES; ++u)
        for (int c = 0; c < STATES; ++c)
            for (int d = 0; d < STATES; ++d)
                mid_valid[u][c][d] = good_middle(u, c, d);
    tables_prepared = true;
}

/* main sequence function */
seq_t a(idx_t n) {
    prepare_tables();
    if (n == 0) return (seq_t)0;        /* undefined below offset */

    /* n == 1 */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < STATES; ++r)
            if (one_row_valid[r]) ans += (seq_t)1;
        return ans / 2;                  /* divide by 2 for complement symmetry */
    }

    /* n == 2 */
    if (n == 2) {
        seq_t ans = 0;
        for (int r0 = 0; r0 < STATES; ++r0)
            for (int r1 = 0; r1 < STATES; ++r1)
                if (first_valid[r0][r1] && last_valid[r0][r1])
                    ans += (seq_t)1;
        return ans / 2;
    }

    /* n >= 3: DP over rows */
    seq_t dp[STATES][STATES] = {{0}};
    seq_t ndp[STATES][STATES] = {{0}};

    /* initial two rows */
    for (int r0 = 0; r0 < STATES; ++r0)
        for (int r1 = 0; r1 < STATES; ++r1)
            if (first_valid[r0][r1])
                dp[r0][r1] += (seq_t)1;

    /* add rows 3 .. n */
    for (idx_t step = 2; step < n; ++step) {
        for (int i = 0; i < STATES; ++i)
            for (int j = 0; j < STATES; ++j)
                ndp[i][j] = (seq_t)0;

        for (int up = 0; up < STATES; ++up)
            for (int cur = 0; cur < STATES; ++cur) {
                seq_t curcnt = dp[up][cur];
                if (curcnt == (seq_t)0) continue;
                for (int down = 0; down < STATES; ++down) {
                    if (!mid_valid[up][cur][down]) continue;
                    ndp[cur][down] += curcnt;
                }
            }

        for (int i = 0; i < STATES; ++i)
            for (int j = 0; j < STATES; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum final states where last row is valid */
    seq_t ans = 0;
    for (int prev = 0; prev < STATES; ++prev)
        for (int cur = 0; cur < STATES; ++cur)
            if (last_valid[prev][cur])
                ans += dp[prev][cur];

    return ans / 2;  /* divide by 2 for complement symmetry */
}
