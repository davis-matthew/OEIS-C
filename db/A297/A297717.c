/*
 * A297717  Number of nX5 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 4 neighboring 1s.
 * https://oeis.org/A297717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays where every 1 has 1, 2, or 4
 *            neighbors among its horizontal (left/right) as well as diagonal
 *            (up‑left, up‑right, down‑left, down‑right) positions (vertical
 *            neighbors are not counted)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Precomputation of bits and transition lists */
static unsigned char bit[32][5];
static unsigned char trans[32][32][32];
static unsigned char trans_len[32][32];
static bool init = false;

static void init_arrays(void)
{
    if (init)
        return;

    /* Precompute bit representation of each 5‑bit pattern. */
    for (int p = 0; p < 32; ++p) {
        for (int j = 0; j < 5; ++j)
            bit[p][j] = (p >> j) & 1U;
    }

    /* Build transition lists: for each pair (prev2,prev), list all cur
       patterns that satisfy the neighbor condition for every 1
       in the row 'prev'. */
    for (int prev2 = 0; prev2 < 32; ++prev2) {
        for (int prev = 0; prev < 32; ++prev) {
            unsigned char len = 0;
            for (int cur = 0; cur < 32; ++cur) {
                bool ok = true;
                for (int j = 0; j < 5; ++j) {
                    if (!bit[prev][j])
                        continue;
                    int cnt = 0;
                    /* left/right in same row */
                    if (j > 0)   cnt += bit[prev][j - 1];
                    if (j < 4)   cnt += bit[prev][j + 1];
                    /* diagonal from prev2 row */
                    if (j > 0)   cnt += bit[prev2][j - 1];
                    if (j < 4)   cnt += bit[prev2][j + 1];
                    /* anti‑diagonal from cur row */
                    if (j > 0)   cnt += bit[cur][j - 1];
                    if (j < 4)   cnt += bit[cur][j + 1];
                    if (cnt != 1 && cnt != 2 && cnt != 4) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    trans[prev2][prev][len++] = (unsigned char)cur;
                }
            }
            trans_len[prev2][prev] = len;
        }
    }

    init = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_arrays();

    /* DP arrays: dp[prev2][prev] holds counts after processing some rows. */
    static seq_t dp[32][32];
    static seq_t next[32][32];

    /* Clear DP arrays */
    memset(dp, 0, sizeof(dp));
    memset(next, 0, sizeof(next));

    /* Start with two dummy rows of zeros. */
    dp[0][0] = 1;

    /* Build the grid row by row. */
    for (idx_t row = 0; row < n; ++row) {
        for (int prev2 = 0; prev2 < 32; ++prev2) {
            for (int prev = 0; prev < 32; ++prev) {
                seq_t val = dp[prev2][prev];
                if (!val)
                    continue;
                unsigned char *list = trans[prev2][prev];
                unsigned char len = trans_len[prev2][prev];
                for (unsigned char k = 0; k < len; ++k) {
                    int cur = list[k];
                    next[prev][cur] += val;
                }
            }
        }
        /* Swap dp and next */
        memcpy(dp, next, sizeof(dp));
        memset(next, 0, sizeof(next));
    }

    /* Final validation: rows after the last one are empty (all 0). */
    seq_t ans = 0;
    for (int prev2 = 0; prev2 < 32; ++prev2) {
        for (int last = 0; last < 32; ++last) {
            seq_t val = dp[prev2][last];
            if (!val)
                continue;

            bool ok = true;
            for (int j = 0; j < 5; ++j) {
                if (!bit[last][j])
                    continue;
                int cnt = 0;
                /* left/right in same row */
                if (j > 0)   cnt += bit[last][j - 1];
                if (j < 4)   cnt += bit[last][j + 1];
                /* diagonal from prev2 row */
                if (j > 0)   cnt += bit[prev2][j - 1];
                if (j < 4)   cnt += bit[prev2][j + 1];
                /* no anti‑diagonal neighbor beyond the last row */
                if (cnt != 1 && cnt != 2 && cnt != 4) {
                    ok = false;
                    break;
                }
            }
            if (ok)
                ans += val;
        }
    }

    return ans;
}
