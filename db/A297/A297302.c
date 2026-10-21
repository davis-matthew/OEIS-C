/*
 * A297302  Number of 5Xn 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 2 neighboring 1s.
 * https://oeis.org/A297302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A297302: Number of 5×n 0..1 arrays where each 1 has exactly two neighbours
 * among the four cells (left, right, up‑left, down‑right).  Computed by
 * a transfer‑matrix dynamic programme over column bitmasks.
 */

#include <stdint.h>
#include <stdbool.h>

#define R        5                      /* number of rows                         */
#define COLS     (1 << R)               /* number of column patterns (32)         */
#define STATES   (COLS * COLS)          /* (prevPrev,prev) pairs (state space)   */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* behaviour below offset ignored        */

    /* ---------- pre‑compute transition tables ---------- */
    static uint8_t trans[STATES][COLS];
    static uint8_t trans_len[STATES];
    static bool initialized = false;

    if (!initialized) {
        for (int s = 0; s < STATES; ++s) {
            int prevPrev = s >> R;       /* higher 5 bits */
            int prev    = s & (COLS - 1);/* lower 5 bits  */
            int cnt = 0;
            for (int cur = 0; cur < COLS; ++cur) {
                bool ok = true;
                for (int r = 0; r < R && ok; ++r) {
                    if (!(prev & (1 << r))) continue;   /* no 1 in this row */
                    int left  = (prevPrev >> r) & 1;
                    if (r + 1 < R) left += (prevPrev >> (r + 1)) & 1;
                    int right = (cur >> r) & 1;
                    if (r > 0)     right += (cur >> (r - 1)) & 1;
                    if (left + right != 2) ok = false;
                }
                if (ok) {
                    trans[s][cnt++] = (uint8_t)cur;
                }
            }
            trans_len[s] = (uint8_t)cnt;
        }
        initialized = true;
    }

    /* ---------- DP arrays ---------- */
    static seq_t dpCurr[STATES];
    static seq_t dpNext[STATES];

    /* initialise: first column masks, virtual left column all zeros */
    for (int i = 0; i < STATES; ++i) dpCurr[i] = 0;
    for (int mask = 0; mask < COLS; ++mask) dpCurr[mask] = (seq_t)1; /* state (0,mask) */

    /* transition over columns (already handled the first column) */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < STATES; ++i) dpNext[i] = 0;
        for (int s = 0; s < STATES; ++s) {
            seq_t val = dpCurr[s];
            if (val == 0) continue;
            int prev = s & (COLS - 1);
            int len  = trans_len[s];
            for (int k = 0; k < len; ++k) {
                int cur = trans[s][k];
                int t = (prev << R) | cur;          /* new state (prev,cur)   */
                dpNext[t] += val;
            }
        }
        for (int i = 0; i < STATES; ++i) dpCurr[i] = dpNext[i];
    }

    /* ---------- final column validation (rightmost column) ---------- */
    seq_t ans = 0;
    for (int s = 0; s < STATES; ++s) {
        seq_t val = dpCurr[s];
        if (val == 0) continue;
        int prevPrev = s >> R;
        int prev     = s & (COLS - 1);
        bool ok = true;
        for (int r = 0; r < R && ok; ++r) {
            if (!(prev & (1 << r))) continue;      /* no 1 in this row */
            int left = (prevPrev >> r) & 1;
            if (r + 1 < R) left += (prevPrev >> (r + 1)) & 1;
            if (left != 2) ok = false;
        }
        if (ok) ans += val;
    }
    return ans;
}
