/*
 * A269081  Number of 7Xn binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of 7×n binary arrays with at most one adjacently‑1 pair across columns
   (adjacency: horizontally, diagonally, or antidiagonally between consecutive columns).   */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Pre‑compute cross‑adjacency counts between mask pairs.
       cross[pm][cm] = 0 (no adjacency),
                    = 1 (exactly one adjacency pair),
                    = 2 (two or more adjacency pairs) */
    static unsigned char cross[128][128];
    static int init = 0;
    if (!init) {
        for (int pm = 0; pm < 128; ++pm) {
            for (int cm = 0; cm < 128; ++cm) {
                int cnt = 0;
                for (int r = 0; r < 7; ++r) {
                    if ((pm >> r) & 1) {
                        if ((cm >> r) & 1) ++cnt;           /* horizontal */
                        if (r > 0       && (cm >> (r - 1)) & 1) ++cnt;  /* diagonal */
                        if (r < 6       && (cm >> (r + 1)) & 1) ++cnt;  /* antidiagonal */
                    }
                }
                cross[pm][cm] = (cnt > 1) ? 2 : (unsigned char)cnt;
            }
        }
        init = 1;
    }

    /* DP arrays: dp0[mask] – sequences ending with mask and 0 adjacency pairs used
       dp1[mask] – sequences ending with mask and exactly 1 adjacency pair used    */
    seq_t dp0[128], dp1[128];
    seq_t new0[128], new1[128];
    for (int i = 0; i < 128; ++i) {
        dp0[i] = 1;   /* first column: no adjacency pairs yet */
        dp1[i] = 0;
    }

    if (n == 1) {                /* sum of dp0 after first column */
        seq_t sum = 0;
        for (int i = 0; i < 128; ++i) sum += dp0[i];
        return sum;
    }

    for (idx_t col = 2; col <= n; ++col) {
        /* zero new arrays */
        for (int i = 0; i < 128; ++i) {
            new0[i] = 0;
            new1[i] = 0;
        }

        for (int pm = 0; pm < 128; ++pm) {
            seq_t val0 = dp0[pm];
            seq_t val1 = dp1[pm];
            for (int cm = 0; cm < 128; ++cm) {
                unsigned char add = cross[pm][cm];
                if (add > 1) continue;           /* too many adjacency pairs */
                if (add == 0) {
                    new0[cm] += val0;           /* still 0 pairs used */
                    new1[cm] += val1;           /* already 1 pair used */
                } else { /* add == 1 */
                    new1[cm] += val0;           /* 0→1 transition */
                }
            }
        }

        /* move new results to dp arrays for next iteration */
        for (int i = 0; i < 128; ++i) {
            dp0[i] = new0[i];
            dp1[i] = new1[i];
        }
    }

    /* total sequences: 0 or 1 adjacency pair used */
    seq_t result = 0;
    for (int i = 0; i < 128; ++i) {
        result += dp0[i];
        result += dp1[i];
    }
    return result;
}
