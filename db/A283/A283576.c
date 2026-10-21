/*
 * A283576  Number of 5Xn 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A283576
   Counts 5×n binary arrays where at most one element may have more than
   one neighbor among the six (left, right, diagonals, antidiagonals).
   The algorithm uses dynamic programming over column masks.
*/
#include <stdbool.h>
#include <stdint.h>

#define NUM_ROWS 5
#define MASKS (1 << NUM_ROWS)

/* precompute, for each triple (prev, cur, next) of column masks,
   the number of cells in cur that would have more than one neighbor
   among those three columns.  Values are 0, 1, or 2 (for ≥2). */
static uint8_t viol[32][32][32];
static bool viol_initialized = false;

static void init_viol(void)
{
    if (viol_initialized) return;

    for (int prev = 0; prev < MASKS; ++prev) {
        for (int cur = 0; cur < MASKS; ++cur) {
            for (int next = 0; next < MASKS; ++next) {
                int cnt_viol = 0;
                for (int r = 0; r < NUM_ROWS; ++r) {
                    if ((cur & (1 << r)) == 0) continue;   /* 1 only */
                    int neigh = 0;
                    /* horizontal neighbours */
                    if (prev & (1 << r)) ++neigh;
                    if (next & (1 << r)) ++neigh;
                    /* diagonal neighbours */
                    if (r > 0) {
                        if (prev & (1 << (r-1))) ++neigh;
                        if (next & (1 << (r-1))) ++neigh;
                    }
                    /* anti‑diagonal neighbours */
                    if (r < NUM_ROWS-1) {
                        if (prev & (1 << (r+1))) ++neigh;
                        if (next & (1 << (r+1))) ++neigh;
                    }
                    if (neigh > 1) {
                        ++cnt_viol;
                        if (cnt_viol > 1) { cnt_viol = 2; break; }
                    }
                }
                viol[prev][cur][next] = (uint8_t)cnt_viol;
            }
        }
    }
    viol_initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    init_viol();

    /* DP arrays:
       dp0[prev][cur] – number of ways with no violation yet
       dp1[prev][cur] – number of ways with exactly one violation already  */
    static seq_t dp0[32][32];
    static seq_t dp1[32][32];
    static seq_t dp0_next[32][32];
    static seq_t dp1_next[32][32];

    /* clear DP arrays for this call */
    for (int i = 0; i < MASKS; ++i)
        for (int j = 0; j < MASKS; ++j) {
            dp0[i][j]   = (seq_t)0;
            dp1[i][j]   = (seq_t)0;
            dp0_next[i][j] = (seq_t)0;
            dp1_next[i][j] = (seq_t)0;
        }

    /* first real column: previous dummy column is 0 */
    for (int cur = 0; cur < MASKS; ++cur)
        dp0[0][cur] = (seq_t)1;

    /* process remaining columns */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MASKS; ++j) {
                dp0_next[i][j] = (seq_t)0;
                dp1_next[i][j] = (seq_t)0;
            }

        for (int prev = 0; prev < MASKS; ++prev) {
            for (int cur  = 0; cur  < MASKS; ++cur) {
                seq_t v0 = dp0[prev][cur];
                if (v0) {
                    for (int nxt = 0; nxt < MASKS; ++nxt) {
                        uint8_t w = viol[prev][cur][nxt];
                        if (w == 0)          dp0_next[cur][nxt] += v0;
                        else if (w == 1)      dp1_next[cur][nxt] += v0;
                    }
                }
                seq_t v1 = dp1[prev][cur];
                if (v1) {
                    for (int nxt = 0; nxt < MASKS; ++nxt) {
                        uint8_t w = viol[prev][cur][nxt];
                        if (w == 0)          dp1_next[cur][nxt] += v1;
                    }
                }
            }
        }

        /* move next to current for next iteration */
        for (int i = 0; i < MASKS; ++i)
            for (int j = 0; j < MASKS; ++j) {
                dp0[i][j] = dp0_next[i][j];
                dp1[i][j] = dp1_next[i][j];
            }
    }

    /* final dummy column 0: count configurations with exactly one violation */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < MASKS; ++prev) {
        for (int cur = 0; cur < MASKS; ++cur) {
            seq_t v0 = dp0[prev][cur];
            if (v0) {
                uint8_t w = viol[prev][cur][0];
                if (w == 1) ans += v0;        /* violation occurs in last column */
            }
            seq_t v1 = dp1[prev][cur];
            if (v1) {
                uint8_t w = viol[prev][cur][0];
                if (w == 0) ans += v1;        /* violation already happened earlier */
            }
        }
    }

    return ans;
}
