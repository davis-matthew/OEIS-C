/*
 * A296154  Number of nX5 0..1 arrays with each 1 adjacent to 3 or 5 king-move
 *          neighboring 1s.
 * https://oeis.org/A296154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A296154: number of n×5 0..1 arrays where
   every 1 has 3 or 5 king‑move neighboring 1s. */
#include <stdbool.h>           /* for bool, true, false */

seq_t a(idx_t n) {
    /*------------------------------------------------------------*/
    /* Precomputation of transition tables (executed once).      */
    /*------------------------------------------------------------*/
    static bool initialized = false;
    static bool valid[32][32][32];
    static unsigned char nextList[1024][32];
    static unsigned char nextCount[1024];
    static bool bottomValid[1024];
    if (!initialized) {
        for (int prev = 0; prev < 32; ++prev) {
            for (int cur = 0; cur < 32; ++cur) {
                int idx = prev << 5 | cur;      /* prev*32 + cur */
                nextCount[idx] = 0;
                for (int next = 0; next < 32; ++next) {
                    bool ok = true;
                    for (int j = 0; j < 5 && ok; ++j) {
                        if ((cur >> j) & 1) {   /* cell (row cur, column j) is 1 */
                            int cnt = 0;
                            if (j > 0) {
                                cnt += (prev >> (j - 1)) & 1;   /* prev diagonal left */
                                cnt += (cur  >> (j - 1)) & 1;   /* cur  left */
                                cnt += (next >> (j - 1)) & 1;   /* next diagonal left */
                            }
                            if (j < 4) {
                                cnt += (prev >> (j + 1)) & 1;   /* prev diagonal right */
                                cnt += (cur  >> (j + 1)) & 1;   /* cur  right */
                                cnt += (next >> (j + 1)) & 1;   /* next diagonal right */
                            }
                            cnt += (prev >> j) & 1;          /* vertical above */
                            cnt += (next >> j) & 1;          /* vertical below */
                            if (!(cnt == 3 || cnt == 5))
                                ok = false;
                        }
                    }
                    valid[prev][cur][next] = ok;
                    if (ok) {
                        nextList[idx][nextCount[idx]++] = (unsigned char)next;
                    }
                }
                bottomValid[idx] = valid[prev][cur][0];
            }
        }
        initialized = true;
    }

    /*------------------------------------------------------------*/
    /* Dynamic programming across rows.                         */
    /*------------------------------------------------------------*/
    static seq_t dp0[1024];
    static seq_t dp1[1024];
    /* clear dp arrays for this call */
    for (int i = 0; i < 1024; ++i) {
        dp0[i] = 0;
        dp1[i] = 0;
    }

    /* Initialize with first row (prev = 0 row of zeros). */
    for (int cur = 0; cur < 32; ++cur) {
        /* index = 0*32 + cur */
        dp0[cur] = (seq_t)1;
    }

    /* Special case n = 1: validate with next = 0 directly. */
    if (n == 1) {
        seq_t ans = 0;
        for (int idx = 0; idx < 1024; ++idx) {
            if (bottomValid[idx])
                ans += dp0[idx];
        }
        return ans;
    }

    seq_t *curdp = dp0;
    seq_t *nxtdp = dp1;

    /* Process rows 2 ... n */
    for (idx_t step = 1; step < n; ++step) {
        /* zero next array */
        for (int i = 0; i < 1024; ++i)
            nxtdp[i] = 0;

        for (int idx = 0; idx < 1024; ++idx) {
            seq_t val = curdp[idx];
            if (val) {
                int prev   = idx & 31;
                int destBase = prev << 5;          /* prev*32 */
                unsigned char cnt = nextCount[idx];
                for (unsigned char k = 0; k < cnt; ++k) {
                    int next = nextList[idx][k];
                    int dest = destBase | next;    /* prev*32 + next */
                    nxtdp[dest] += val;
                }
            }
        }
        /* swap arrays */
        seq_t *tmp  = curdp;
        curdp = nxtdp;
        nxtdp = tmp;
    }

    /* Final row validation with next = 0 (bottom border). */
    seq_t ans = 0;
    for (int idx = 0; idx < 1024; ++idx) {
        if (bottomValid[idx])
            ans += curdp[idx];
    }
    return ans;
}
