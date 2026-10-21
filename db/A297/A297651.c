/*
 * A297651  Number of nX5 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 3 neighboring 1s.
 * https://oeis.org/A297651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A297651: 3‑row transfer‑matrix DP
 * Number of n×5 binary arrays with every 1 horizontally, diagonally
 * or antidiagonally adjacent to 1, 2 or 3 neighboring 1's.
 * No closed formula is known; the program enumerates row patterns
 * and uses dynamic programming over the last two rows. */

#include <stdbool.h>
#include <stdint.h>

#define WIDTH 5
#define STATES (1 << WIDTH)   /* 32 */

static uint8_t ok_triple[STATES][STATES][STATES];
static uint8_t next_cnt[STATES][STATES];
static uint8_t next_rows[STATES][STATES][STATES];

/* Test whether the middle row satisfies the constraint with its
 * upper and lower neighbours. */
static inline bool triple_ok(int up, int mid, int down)
{
    for (int col = 0; col < WIDTH; ++col) {
        if ((mid >> col) & 1) {
            int cnt = 0;
            /* horizontal neighbours in the same row */
            if (col > 0     && ((mid >> (col - 1)) & 1)) ++cnt;
            if (col < WIDTH-1 && ((mid >> (col + 1)) & 1)) ++cnt;
            /* diagonal neighbours in previous row */
            if (col > 0     && ((up   >> (col - 1)) & 1)) ++cnt;
            if (col < WIDTH-1 && ((up   >> (col + 1)) & 1)) ++cnt;
            /* diagonal neighbours in next row */
            if (col > 0     && ((down >> (col - 1)) & 1)) ++cnt;
            if (col < WIDTH-1 && ((down >> (col + 1)) & 1)) ++cnt;
            if (cnt < 1 || cnt > 3) return false;
        }
    }
    return true;
}

/* Build lookup tables once. */
static void init_tables(void)
{
    for (int up = 0; up < STATES; ++up) {
        for (int mid = 0; mid < STATES; ++mid) {
            int cnt = 0;
            for (int down = 0; down < STATES; ++down) {
                bool ok = triple_ok(up, mid, down);
                ok_triple[up][mid][down] = ok;
                if (ok) next_rows[up][mid][cnt++] = (uint8_t)down;
            }
            next_cnt[up][mid] = (uint8_t)cnt;
        }
    }
}

/* The required OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1 */

    static bool tables_ready = false;
    if (!tables_ready) {
        init_tables();
        tables_ready = true;
    }

    /* n == 1 is special */
    if (n == 1) {
        seq_t total = 0;
        for (int r = 0; r < STATES; ++r)
            if (ok_triple[0][r][0]) ++total;
        return total;
    }

    /* DP arrays: dp[prev][curr] = number of partial arrays. */
    seq_t dp[STATES][STATES] = {0};
    seq_t ndp[STATES][STATES] = {0};

    /* Initialise with the first two rows. */
    for (int r0 = 0; r0 < STATES; ++r0)
        for (int r1 = 0; r1 < STATES; ++r1)
            if (ok_triple[0][r0][r1])
                dp[r0][r1] = 1;

    /* Process rows 2 … n-1 (zero‑based). */
    for (idx_t i = 2; i < n; ++i) {
        /* reset ndp */
        for (int a = 0; a < STATES; ++a)
            for (int b = 0; b < STATES; ++b)
                ndp[a][b] = 0;

        for (int prev = 0; prev < STATES; ++prev) {
            for (int curr = 0; curr < STATES; ++curr) {
                seq_t cnt = dp[prev][curr];
                if (!cnt) continue;
                int max_next = next_cnt[prev][curr];
                for (int k = 0; k < max_next; ++k) {
                    int next = next_rows[prev][curr][k];
                    ndp[curr][next] += cnt;
                }
            }
        }

        /* move ndp into dp for the next iteration */
        for (int a = 0; a < STATES; ++a)
            for (int b = 0; b < STATES; ++b)
                dp[a][b] = ndp[a][b];
    }

    /* Final row must satisfy constraints with trailing zeros. */
    seq_t result = 0;
    for (int prev = 0; prev < STATES; ++prev) {
        for (int curr = 0; curr < STATES; ++curr) {
            seq_t cnt = dp[prev][curr];
            if (!cnt) continue;
            if (ok_triple[prev][curr][0]) result += cnt;
        }
    }
    return result;
}
