/*
 * A269105  Number of nX4 0..3 arrays with some element plus some horizontally
 *          or vertically adjacent neighbor totalling three no more than once.
 * https://oeis.org/A269105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269105: Count of n×4 arrays over {0,1,2,3} with at most one
 * horizontally or vertically adjacent pair summing to 3. */
#include <stddef.h>
#include <stdbool.h>

/* Pre‑computed tables: horizontal sum‑3 count in a row,
 * vertical sum‑3 count between two rows, and digit decompositions. */
static unsigned char horiz[256];
static unsigned char vert[256][256];
static uint8_t digits[256][4];
static bool tables_ready = false;

static void init_tables(void)
{
    if (tables_ready) return;
    for (int r = 0; r < 256; ++r) {
        int tmp = r;
        for (int j = 0; j < 4; ++j) {
            digits[r][j] = tmp & 3;
            tmp >>= 2;
        }
        unsigned char h = 0;
        for (int j = 0; j < 3; ++j)
            if (digits[r][j] + digits[r][j+1] == 3) ++h;
        horiz[r] = h;
    }
    for (int r1 = 0; r1 < 256; ++r1) {
        for (int r2 = 0; r2 < 256; ++r2) {
            unsigned char v = 0;
            for (int j = 0; j < 4; ++j)
                if (digits[r1][j] + digits[r2][j] == 3) ++v;
            vert[r1][r2] = v;
        }
    }
    tables_ready = true;
}

seq_t a(idx_t n)
{
    init_tables();
    /* DP arrays: dp[r][u] for one row r and u (0 or 1) pairs already used. */
    seq_t dp_prev[256][2] = {{0}};  /* zero-initialised */
    seq_t dp_next[256][2] = {{0}};

    /* Initialise for the first row. */
    for (int r = 0; r < 256; ++r) {
        unsigned char h = horiz[r];
        if (h <= 1)
            dp_prev[r][h] = (seq_t)1;
    }

    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < 256; ++r)
            ans += dp_prev[r][0] + dp_prev[r][1];
        return ans;
    }

    /* Process subsequent rows. */
    for (idx_t step = 2; step <= n; ++step) {
        /* Clear dp_next. */
        for (int r = 0; r < 256; ++r) dp_next[r][0] = dp_next[r][1] = 0;

        for (int r0 = 0; r0 < 256; ++r0) {
            seq_t cnt0 = dp_prev[r0][0];
            seq_t cnt1 = dp_prev[r0][1];
            if (cnt0 == 0 && cnt1 == 0) continue;

            unsigned char h0 = horiz[0]; /* unused, just for style */

            for (int r1 = 0; r1 < 256; ++r1) {
                unsigned char delta = vert[r0][r1] + horiz[r1];

                /* Transition from state used=0. */
                if (cnt0 != 0) {
                    unsigned char newu = delta;
                    if (newu <= 1)
                        dp_next[r1][newu] += cnt0;
                }
                /* Transition from state used=1. */
                if (cnt1 != 0) {
                    if (delta != 0) continue;  /* would exceed limit */
                    dp_next[r1][1] += cnt1;
                }
            }
        }
        /* Swap dp_prev and dp_next for next iteration. */
        for (int r = 0; r < 256; ++r) {
            dp_prev[r][0] = dp_next[r][0];
            dp_prev[r][1] = dp_next[r][1];
        }
    }

    /* Sum all counts with used 0 or 1 after last row. */
    seq_t answer = 0;
    for (int r = 0; r < 256; ++r)
        answer += dp_prev[r][0] + dp_prev[r][1];
    return answer;
}
