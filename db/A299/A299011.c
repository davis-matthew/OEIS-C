/*
 * A299011  Number of nX4 0..1 arrays with every element equal to 0, 1, 2, 4,
 *          5 or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A299011: Number of n×4 binary arrays (top‑left = 0) such that
 *  for every cell the number of adjacent (king‑move) cells having the
 *  same value equals 0, 1, 2, 4, 5 or 6.
 *  The function uses dynamic programming over the last two rows.
 *  Complexity:  O(n·16²·16) ≈ O(n),  < 2 s for n ≈ 10000.
 */

#include <stdint.h>
#include <stdbool.h>

static const bool allowed[9] = {true,true,true,false,true,true,true,false,false};

static bool valid_top[16][16];
static bool valid_mid[16][16][16];
static bool valid_bottom[16][16];
static bool tables_ready = false;

static void compute_tables(void)
{
    if (tables_ready) return;

    /* top row (no upper neighbours) */
    for (int p0 = 0; p0 < 16; ++p0)
        for (int p1 = 0; p1 < 16; ++p1) {
            bool ok = true;
            for (int c = 0; c < 4 && ok; ++c) {
                int v   = (p0 >> c) & 1;
                int cnt = 0;
                if (c > 0) cnt += ((p0 >> (c-1)) & 1) == v;
                if (c < 3) cnt += ((p0 >> (c+1)) & 1) == v;
                if (c > 0) cnt += ((p1 >> (c-1)) & 1) == v;
                cnt += ((p1 >> c) & 1) == v;
                if (c < 3) cnt += ((p1 >> (c+1)) & 1) == v;
                if (!allowed[cnt]) { ok = false; break; }
            }
            valid_top[p0][p1] = ok;
        }

    /* middle rows (upper, centre, lower exist) */
    for (int up = 0; up < 16; ++up)
        for (int cur = 0; cur < 16; ++cur)
            for (int down = 0; down < 16; ++down) {
                bool ok = true;
                for (int c = 0; c < 4 && ok; ++c) {
                    int v   = (cur >> c) & 1;
                    int cnt = 0;
                    if (c > 0) cnt += ((up   >> (c-1)) & 1) == v;
                    cnt += ((up   >> c) & 1) == v;
                    if (c < 3) cnt += ((up   >> (c+1)) & 1) == v;
                    if (c > 0) cnt += ((cur  >> (c-1)) & 1) == v;
                    if (c < 3) cnt += ((cur  >> (c+1)) & 1) == v;
                    if (c > 0) cnt += ((down >> (c-1)) & 1) == v;
                    cnt += ((down >> c) & 1) == v;
                    if (c < 3) cnt += ((down >> (c+1)) & 1) == v;
                    if (!allowed[cnt]) { ok = false; break; }
                }
                valid_mid[up][cur][down] = ok;
            }

    /* bottom row (no lower neighbours) */
    for (int up = 0; up < 16; ++up)
        for (int cur = 0; cur < 16; ++cur) {
            bool ok = true;
            for (int c = 0; c < 4 && ok; ++c) {
                int v   = (cur >> c) & 1;
                int cnt = 0;
                if (c > 0) cnt += ((cur  >> (c-1)) & 1) == v;
                if (c < 3) cnt += ((cur  >> (c+1)) & 1) == v;
                if (c > 0) cnt += ((up   >> (c-1)) & 1) == v;
                cnt += ((up   >> c) & 1) == v;
                if (c < 3) cnt += ((up   >> (c+1)) & 1) == v;
                if (!allowed[cnt]) { ok = false; break; }
            }
            valid_bottom[up][cur] = ok;
        }

    tables_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    compute_tables();

    /* Special case: single row (no top or bottom neighbours) */
    if (n == 1) {
        seq_t total = 0;
        for (int p0 = 0; p0 < 16; ++p0) {
            if ((p0 & 1) != 0) continue;   /* top‑left must be 0 */
            bool ok = true;
            for (int c = 0; c < 4 && ok; ++c) {
                int v   = (p0 >> c) & 1;
                int cnt = 0;
                if (c > 0) cnt += ((p0 >> (c-1)) & 1) == v;
                if (c < 3) cnt += ((p0 >> (c+1)) & 1) == v;
                if (!allowed[cnt]) { ok = false; break; }
            }
            if (ok) total += 1;
        }
        return total;
    }

    /* DP over last two rows */
    static seq_t dp[16][16];
    static seq_t next[16][16];

    /* initialise dp with first two rows */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;

    for (int p0 = 0; p0 < 16; ++p0) {
        if ((p0 & 1) != 0) continue;                 /* top‑left 0 */
        for (int p1 = 0; p1 < 16; ++p1)
            if (valid_top[p0][p1]) dp[p0][p1] = 1;
    }

    /* process remaining rows */
    for (idx_t row = 2; row < n; ++row) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                next[i][j] = 0;

        for (int up = 0; up < 16; ++up)
            for (int cur = 0; cur < 16; ++cur) {
                seq_t cnt = dp[up][cur];
                if (!cnt) continue;
                for (int down = 0; down < 16; ++down)
                    if (valid_mid[up][cur][down])
                        next[cur][down] += cnt;
            }

        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = next[i][j];
    }

    /* final check for the last row */
    seq_t total = 0;
    for (int up = 0; up < 16; ++up)
        for (int cur = 0; cur < 16; ++cur) {
            seq_t cnt = dp[up][cur];
            if (!cnt) continue;
            if (valid_bottom[up][cur]) total += cnt;
        }

    return total;
}
