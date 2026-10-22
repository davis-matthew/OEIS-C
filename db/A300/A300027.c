/*
 * A300027  Number of nX5 0..1 arrays with every element equal to 0, 1, 3, 4,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A300027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A300027: DP for n×5 binary arrays with king‑move neighbor constraints, first entry 0. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define ROWS      32            /* 2^5 possible rows (5 bits) */
#define SENTINEL  ROWS          /* special value for “no row” */
#define STATES   ((SENTINEL+1)*ROWS)

/* allowed neighbor counts for each cell: 0,1,3,4,6,7,8 */
static const bool ALLOWED[9] = {
    true, /* 0 */
    true, /* 1 */
    false,/* 2 */
    true, /* 3 */
    true, /* 4 */
    false,/* 5 */
    true, /* 6 */
    true, /* 7 */
    true  /* 8 */
};

/* pre-computed allowed next rows for each (prev,row) pair. */
static unsigned char trans_cnt[SENTINEL+1][ROWS];
static int trans_next[SENTINEL+1][ROWS][ROWS];

/* helper to check constraints for the middle row cur using rows
   prev (possibly SENTINEL) and next (possibly SENTINEL). */
static bool ok_row(int prev, int cur, int next) {
    const bool prev_ok = prev != SENTINEL;
    const bool next_ok = next != SENTINEL;
    for (int col = 0; col < 5; ++col) {
        int val = (cur >> col) & 1;
        int cnt = 0;

        /* same row neighbours */
        if (col > 0 && ((cur >> (col-1)) & 1) == val) ++cnt;
        if (col < 4 && ((cur >> (col+1)) & 1) == val) ++cnt;

        /* previous row neighbours */
        if (prev_ok) {
            if (col > 0 && ((prev >> (col-1)) & 1) == val) ++cnt;
            if (((prev >> col) & 1) == val) ++cnt;
            if (col < 4 && ((prev >> (col+1)) & 1) == val) ++cnt;
        }

        /* next row neighbours */
        if (next_ok) {
            if (col > 0 && ((next >> (col-1)) & 1) == val) ++cnt;
            if (((next >> col) & 1) == val) ++cnt;
            if (col < 4 && ((next >> (col+1)) & 1) == val) ++cnt;
        }

        if (!ALLOWED[cnt]) return false;
    }
    return true;
}

/* run once to fill trans arrays. */
static void init_trans(void) {
    static bool done = false;
    if (done) return;
    done = true;
    for (int prev = 0; prev <= SENTINEL; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            unsigned char k = 0;
            for (int nxt = 0; nxt < ROWS; ++nxt) {
                if (ok_row(prev, cur, nxt)) {
                    trans_next[prev][cur][k++] = nxt;
                }
            }
            trans_cnt[prev][cur] = k;
        }
    }
}

/* DP arrays */
static seq_t dp[STATES];
static seq_t dp_next[STATES];

seq_t a(idx_t n) {
    if (n == 0) return 0;

    init_trans();

    /* clear dp arrays */
    for (size_t i = 0; i < STATES; ++i) dp[i] = 0;

    /* initial state: first row patterns with upper‑left zero */
    for (int p = 0; p < ROWS; ++p) {
        if (p & 1) continue;             /* upper‑left must be 0 */
        dp[SENTINEL*ROWS + p] = 1;        /* prev = SENTINEL, cur = p */
    }

    /* process rows 1 .. n-1 */
    for (idx_t row = 1; row < n; ++row) {
        /* reset dp_next */
        for (size_t i = 0; i < STATES; ++i) dp_next[i] = 0;

        for (int prev = 0; prev <= SENTINEL; ++prev) {
            for (int cur = 0; cur < ROWS; ++cur) {
                seq_t ways = dp[prev*ROWS + cur];
                if (!ways) continue;

                unsigned char k = trans_cnt[prev][cur];
                for (unsigned char t = 0; t < k; ++t) {
                    int nxt = trans_next[prev][cur][t];
                    dp_next[cur*ROWS + nxt] += ways;
                }
            }
        }

        /* swap dp and dp_next by copy */
        for (size_t i = 0; i < STATES; ++i) dp[i] = dp_next[i];
    }

    /* final answer */
    seq_t ans = 0;
    for (int prev = 0; prev <= SENTINEL; ++prev) {
        for (int cur = 0; cur < ROWS; ++cur) {
            seq_t ways = dp[prev*ROWS + cur];
            if (!ways) continue;
            if (ok_row(prev, cur, SENTINEL))
                ans += ways;
        }
    }
    return ans;
}
