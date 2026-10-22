/*
 * A301397  Number of nX5 0..1 arrays with every element equal to 2, 3, 4, 5
 *          or 6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A301397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Code computes the A301397 sequence:  
   a(n) = number of n×5 binary matrices with the upper‑left element 0
          such that each element has 2–6 equal neighbours (horizontally,
          vertically or antidiagonally).  The method is a DP over rows –
          no closed‑form formula. */
#define NUM_COLS 5
#define NUM_PAT  (1 << NUM_COLS)

static unsigned char bits[NUM_PAT][NUM_COLS];
static unsigned char nextList[NUM_PAT][NUM_PAT][NUM_PAT];
static unsigned char nextCnt[NUM_PAT][NUM_PAT];
static unsigned char nextNoPrevList[NUM_PAT][NUM_PAT];
static unsigned char nextNoPrevCnt[NUM_PAT];
static bool validNoNext[NUM_PAT][NUM_PAT];
static bool validNoPrevNoNext[NUM_PAT];
static bool tables_ready = false;

static inline bool row_valid(int prev, int curr, int next,
                             bool has_prev, bool has_next)
{
    const unsigned char *b  = bits[curr];
    const unsigned char *bp = has_prev ? bits[prev] : NULL;
    const unsigned char *bn = has_next ? bits[next] : NULL;

    for (int j = 0; j < NUM_COLS; ++j) {
        unsigned val = b[j];
        int cnt = 0;
        /* left/right */
        if (j > 0 && b[j - 1] == val) ++cnt;
        if (j < NUM_COLS - 1 && b[j + 1] == val) ++cnt;
        /* up/down */
        if (has_prev && bp[j] == val) ++cnt;
        if (has_next && bn[j] == val) ++cnt;
        /* anti‑diagonals */
        if (has_prev && j < NUM_COLS - 1 && bp[j + 1] == val) ++cnt;
        if (has_next && j > 0 && bn[j - 1] == val) ++cnt;
        if (cnt < 2 || cnt > 6) return false;
    }
    return true;
}

static void build_tables(void)
{
    if (tables_ready) return;

    /* pattern bits */
    for (int p = 0; p < NUM_PAT; ++p) {
        for (int j = 0; j < NUM_COLS; ++j) {
            bits[p][j] = (p >> j) & 1;
        }
    }

    /* transitions for (prev,curr) -> next */
    for (int prev = 0; prev < NUM_PAT; ++prev) {
        for (int curr = 0; curr < NUM_PAT; ++curr) {
            unsigned char m = 0;
            for (int next = 0; next < NUM_PAT; ++next) {
                if (row_valid(prev, curr, next, true, true)) {
                    nextList[prev][curr][m++] = (unsigned char)next;
                }
            }
            nextCnt[prev][curr] = m;
        }
    }

    /* transitions from top row (no previous) */
    for (int curr = 0; curr < NUM_PAT; ++curr) {
        unsigned char m = 0;
        for (int next = 0; next < NUM_PAT; ++next) {
            if (row_valid(-1, curr, next, false, true)) {
                nextNoPrevList[curr][m++] = (unsigned char)next;
            }
        }
        nextNoPrevCnt[curr] = m;
    }

    /* last row validity (no next) */
    for (int prev = 0; prev < NUM_PAT; ++prev) {
        for (int curr = 0; curr < NUM_PAT; ++curr) {
            validNoNext[prev][curr] = row_valid(prev, curr, -1, true, false);
        }
    }

    /* single row validity (no prev, no next) */
    for (int curr = 0; curr < NUM_PAT; ++curr) {
        validNoPrevNoNext[curr] = row_valid(-1, curr, -1, false, false);
    }

    tables_ready = true;
}

seq_t a(idx_t n)
{
    build_tables();

    if (n == 0) return 0; /* safety, offset starts at 1 */

    /* DP arrays */
    seq_t dp[NUM_PAT][NUM_PAT];
    seq_t ndp[NUM_PAT][NUM_PAT];

    for (int i = 0; i < NUM_PAT; ++i)
        for (int j = 0; j < NUM_PAT; ++j)
            dp[i][j] = 0;

    if (n == 1) {
        seq_t result = 0;
        for (int r = 0; r < NUM_PAT; ++r) {
            if (bits[r][0] == 0 && validNoPrevNoNext[r])
                result += (seq_t)1;
        }
        return result;
    }

    /* n >= 2: initialise with top row and second row */
    for (int r1 = 0; r1 < NUM_PAT; ++r1) {
        if (bits[r1][0] != 0) continue; /* top‑left must be 0 */
        unsigned char m = nextNoPrevCnt[r1];
        for (unsigned char k = 0; k < m; ++k) {
            int r2 = nextNoPrevList[r1][k];
            dp[r1][r2] += (seq_t)1;
        }
    }

    /* process rows 3 .. n */
    if (n >= 3) {
        for (idx_t row = 3; row <= n; ++row) {
            for (int i = 0; i < NUM_PAT; ++i)
                for (int j = 0; j < NUM_PAT; ++j)
                    ndp[i][j] = 0;

            for (int prev = 0; prev < NUM_PAT; ++prev) {
                for (int curr = 0; curr < NUM_PAT; ++curr) {
                    seq_t cnt = dp[prev][curr];
                    if (cnt == 0) continue;
                    unsigned char m = nextCnt[prev][curr];
                    for (unsigned char k = 0; k < m; ++k) {
                        int nxt = nextList[prev][curr][k];
                        ndp[curr][nxt] += cnt;
                    }
                }
            }

            for (int i = 0; i < NUM_PAT; ++i)
                for (int j = 0; j < NUM_PAT; ++j)
                    dp[i][j] = ndp[i][j];
        }
    }

    /* sum over valid final states */
    seq_t result = 0;
    for (int prev = 0; prev < NUM_PAT; ++prev) {
        for (int curr = 0; curr < NUM_PAT; ++curr) {
            seq_t cnt = dp[prev][curr];
            if (cnt == 0) continue;
            if (validNoNext[prev][curr])
                result += cnt;
        }
    }
    return result;
}
