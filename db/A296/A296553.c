/*
 * A296553  Number of nX7 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 3 or 6 neighboring 1s.
 * https://oeis.org/A296553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 0/1 arrays where each 1 is horizontally,
 *            vertically, or antidiagonally adjacent to either 3 or 6
 *            neighboring 1s. Computed by dynamic programming over 7‑bit
 *            column patterns
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Problem parameters */
#define COLS 7
#define MAX_COL 128                 /* 2^7 possible column patterns */
#define STATE_COUNT (MAX_COL * MAX_COL)

/* Pre‑computed tables */
static bool                    valid_right[MAX_COL][MAX_COL];      /* right neighbour 0 */
static uint8_t                 next_list[MAX_COL][MAX_COL][MAX_COL];
static uint8_t                 next_len[MAX_COL][MAX_COL];
static bool                    precomp_done = false;

/* Test whether the middle column in a 3‑column window is valid */
static bool validTriplet(uint8_t a, uint8_t b, uint8_t c)
{
    for (int r = 0; r < COLS; ++r) {
        if ((b >> r) & 1) {
            int cnt = 0;
            /* vertical neighbours */
            if (r > 0 && (b >> (r - 1)) & 1) ++cnt;
            if (r < COLS - 1 && (b >> (r + 1)) & 1) ++cnt;
            /* horizontal neighbours */
            if ((a >> r) & 1) ++cnt;      /* left */
            if ((c >> r) & 1) ++cnt;      /* right */
            /* antidiagonal neighbours */
            if (r > 0 && (c >> (r - 1)) & 1) ++cnt;     /* up‑right */
            if (r < COLS - 1 && (a >> (r + 1)) & 1) ++cnt;  /* down‑left */
            if (cnt != 3 && cnt != 6)
                return false;
        }
    }
    return true;
}

/* Fill transition tables once */
static void precompute(void)
{
    if (precomp_done)
        return;
    precomp_done = true;
    for (uint8_t a = 0; a < MAX_COL; ++a) {
        for (uint8_t b = 0; b < MAX_COL; ++b) {
            valid_right[a][b] = validTriplet(a, b, 0);
            uint8_t len = 0;
            for (uint8_t c = 0; c < MAX_COL; ++c) {
                if (validTriplet(a, b, c))
                    next_list[a][b][len++] = c;
            }
            next_len[a][b] = len;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    precompute();                   /* ensure tables are ready */

    if (n == 0)                     /* offset starts at 1 */
        return 0;

    /* DP tables */
    static seq_t dp0[STATE_COUNT];
    static seq_t dp1[STATE_COUNT];
    static seq_t *dp_cur = dp0;
    static seq_t *dp_next = dp1;

    /* initialise DP with the first column (left neighbour = 0) */
    memset(dp_cur, 0, sizeof(seq_t) * STATE_COUNT);
    for (int c = 0; c < MAX_COL; ++c)
        dp_cur[c] = 1;

    /* answer after first column */
    seq_t ans = 0;
    for (int c = 0; c < MAX_COL; ++c)
        if (valid_right[0][c])
            ans += dp_cur[c];

    if (n == 1)
        return ans;

    /* iterate over columns 2 .. n */
    for (idx_t col = 2; col <= n; ++col) {
        memset(dp_next, 0, sizeof(seq_t) * STATE_COUNT);

        for (int prev = 0; prev < MAX_COL; ++prev) {
            for (int curr = 0; curr < MAX_COL; ++curr) {
                seq_t cnt = dp_cur[prev * MAX_COL + curr];
                if (!cnt)
                    continue;
                uint8_t len = next_len[prev][curr];
                const uint8_t *list = next_list[prev][curr];
                for (uint8_t k = 0; k < len; ++k) {
                    uint8_t nxt = list[k];
                    dp_next[curr * MAX_COL + nxt] += cnt;
                }
            }
        }

        /* swap DP tables */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* compute answer for this width */
        ans = 0;
        for (int prev = 0; prev < MAX_COL; ++prev) {
            for (int curr = 0; curr < MAX_COL; ++curr) {
                if (valid_right[prev][curr])
                    ans += dp_cur[prev * MAX_COL + curr];
            }
        }
    }
    return ans;
}
