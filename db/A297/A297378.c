/*
 * A297378  Number of 5Xn 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 0, 1 or 2 neighboring 1s.
 * https://oeis.org/A297378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A297378 - Number of 5×n 0..1 arrays with every 1 having at most
    2 horizontally or antidiagonally adjacent 1s.
    The algorithm uses a transfer‑matrix DP over column masks.
*/

#include <stdbool.h>
#include <string.h>

/*  Precomputation tables  ------------------------------------------------- */
static bool _initialized = false;
static unsigned char _leftCnt[32][5];      /* bits at rows r and r+1 of prev mask */
static unsigned char _rightCnt[32][5];     /* bits at rows r and r-1 of next mask */
static int _transCnt[32][32];
static int  _transNext[32][32][32];

/*  Initialise the tables once  ------------------------------------------- */
static void _init_tables(void)
{
    if (_initialized) return;
    _initialized = true;

    /* left and right neighbour count for each mask and row */
    for (int m = 0; m < 32; ++m) {
        for (int r = 0; r < 5; ++r) {
            unsigned char c = 0;
            if (m & (1U << r))                       c++;   /* same row (prev) */
            if (r < 4 && (m & (1U << (r + 1))))     c++;   /* row below   (prev) */
            _leftCnt[m][r] = c;

            unsigned char rc = 0;
            if (m & (1U << r))                      rc++;          /* same row (next) */
            if (r > 0 && (m & (1U << (r - 1))))     rc++;          /* row above (next) */
            _rightCnt[m][r] = rc;
        }
    }

    /* For each pair (prev,cur) pre‑compute all valid nxt masks */
    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            int cnt = 0;
            for (int nxt = 0; nxt < 32; ++nxt) {
                bool ok = true;
                for (int r = 0; r < 5 && ok; ++r) {
                    if (cur & (1U << r)) {
                        int left  = _leftCnt[prev][r];
                        int right = _rightCnt[nxt][r];
                        if (left + right > 2) ok = false;
                    }
                }
                if (ok) _transNext[prev][cur][cnt++] = nxt;
            }
            _transCnt[prev][cur] = cnt;
        }
    }
}

/*  Main sequence routine  ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    if (n == 1) return (seq_t)32;          /* all 5‑bit masks */

    _init_tables();

    static seq_t dp[32][32];
    static seq_t dp_next[32][32];

    /* initialise for column 1: prev mask = 0 (imaginary left border) */
    memset(dp, 0, sizeof(dp));
    for (int cur = 0; cur < 32; ++cur) dp[0][cur] = (seq_t)1;

    /* process columns 2 … n */
    for (idx_t col = 2; col <= n; ++col) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int prev = 0; prev < 32; ++prev) {
            for (int cur = 0; cur < 32; ++cur) {
                seq_t val = dp[prev][cur];
                if (val == 0) continue;
                int tc = _transCnt[prev][cur];
                int *list = _transNext[prev][cur];
                for (int k = 0; k < tc; ++k) {
                    int nxt = list[k];
                    dp_next[cur][nxt] += val;
                }
            }
        }
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* sum only states where the last column cells have no right neighbour
       (i.e., their left neighbour count ≤ 2) */
    seq_t total = (seq_t)0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            seq_t val = dp[prev][cur];
            if (val == 0) continue;
            bool ok = true;
            for (int r = 0; r < 5 && ok; ++r) {
                if (cur & (1U << r)) {
                    if (_leftCnt[prev][r] > 2) ok = false;
                }
            }
            if (ok) total += val;
        }
    }
    return total;
}
