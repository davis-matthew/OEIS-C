/*
 * A252616  Number of (1+2) X (n+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 3 5 6 or 8 and every 3X3 column and
 *          antidiagonal sum not equal to 0 3 5 6 or 8.
 * https://oeis.org/A252616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A252616: number of (1+2)×(n+2) arrays over {0,1,2,3}
    with every 3×3 subblock having
        - each row sum and the main diagonal sum belong to {0,3,5,6,8}
        - each column sum and the anti‑diagonal sum not belong to that set.
    The function uses a transfer‑matrix DP with states being the last two
    columns (encoded as 6‑bit numbers).  It runs in O(n) time and uses only
    integer arithmetic.  The table of allowed sums is corrected and the
    anti‑diagonal is treated as a forbidden sum. */
#include <stdint.h>

static int preinit_done = 0;
static uint8_t next_count[4096];
static uint8_t next_col[4096][64];
static uint8_t col0[64], col1[64], col2[64], colsum[64];

/* 1 indicates sum is in {0,3,5,6,8}, otherwise 0.  Index is the sum value */
static const int allowed[13] = {
    1, /* 0 */
    0, /* 1 */
    0, /* 2 */
    1, /* 3 */
    0, /* 4 */
    1, /* 5 */
    1, /* 6 */
    0, /* 7 */
    1, /* 8 */
    0, /* 9 */
    0, /*10*/
    0, /*11*/
    0  /*12*/
};

static void preinit(void)
{
    int a,b,c;

    /* digits of each column (base‑4 number 0..63) */
    for (a = 0; a < 64; ++a) {
        col0[a]   = a & 3;          /* top row         */
        col1[a]   = (a >> 2) & 3;   /* middle row      */
        col2[a]   = (a >> 4) & 3;   /* bottom row      */
        colsum[a] = col0[a] + col1[a] + col2[a];
    }

    /* build transition table */
    for (a = 0; a < 64; ++a) {
        for (b = 0; b < 64; ++b) {
            int idx = a * 64 + b;
            int cnt = 0;
            for (c = 0; c < 64; ++c) {
                /* row sums must be allowed */
                if (!allowed[col0[a] + col0[b] + col0[c]]) continue;
                if (!allowed[col1[a] + col1[b] + col1[c]]) continue;
                if (!allowed[col2[a] + col2[b] + col2[c]]) continue;

                /* column sums must NOT be allowed */
                if (allowed[colsum[a]]) continue;
                if (allowed[colsum[b]]) continue;
                if (allowed[colsum[c]]) continue;

                /* main diagonal must be allowed */
                if (!allowed[col0[a] + col1[b] + col2[c]]) continue;

                /* anti‑diagonal must NOT be allowed */
                if (allowed[col2[a] + col1[b] + col0[c]]) continue;

                next_col[idx][cnt++] = (uint8_t)c;
                if (cnt == 64) break;   /* safety */
            }
            next_count[idx] = (uint8_t)cnt;
        }
    }
    preinit_done = 1;
}

seq_t a(idx_t n)
{
    /* number of columns */
    idx_t w = n + 2;          /* must be >= 3 for n >= 1 */
    if (w < 3) return 0;

    if (!preinit_done) preinit();

    static seq_t dp[4096];
    static seq_t ndp[4096];
    int i, k, step, s, c;

    /* initial state: any pair of first two columns */
    for (i = 0; i < 4096; ++i) dp[i] = 1;

    step = 0;
    while (step < (int)(w - 2)) {
        /* zero ndp */
        for (i = 0; i < 4096; ++i) ndp[i] = 0;

        /* transition */
        for (s = 0; s < 4096; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            int cnt = next_count[s];
            for (k = 0; k < cnt; ++k) {
                c = next_col[s][k];
                int idx = (s % 64) * 64 + c;  /* new state (b,c) */
                ndp[idx] += val;
            }
        }

        /* swap dp and ndp */
        for (i = 0; i < 4096; ++i) dp[i] = ndp[i];
        ++step;
    }

    /* sum over all ending states */
    seq_t result = 0;
    for (i = 0; i < 4096; ++i) result += dp[i];
    return result;
}
