/*
 * A297512  Number of 7Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A297512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n binary arrays where each 1 has exactly 1 or
 *            2 neighbours horizontally, diagonally, or antidiagonally
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a transfer‑matrix DP across columns.
   State = 7 rows × 2 bits per row (0–3) where:
      0 = cell is 0
      1 = cell is 1 with 1 neighbour from the left side
      2 = cell is 1 with 2 neighbours from the left side
      3 = cell is 1 with 0 neighbours from the left side
   The DP transitions use only left and right neighbours (no vertical ones). */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Maximum number of states: 2 bits per row × 7 rows = 14 bits = 16384 */
    enum { STATE_BITS = 14, STATES = 1 << STATE_BITS };
    static uint8_t val[STATES][7];                /* per state row values 0..3   */
    static int      transCnt[STATES];             /* number of outgoing transitions */
    static size_t   offsetST[STATES + 1];         /* offsets in flat array        */
    static uint16_t *transFlat = NULL;            /* flattened transition list   */
    static uint8_t  validEnd[STATES];             /* final‑column validity flag  */
    static int      initialized = 0;

    if (!initialized) {
        /* ---------- decode row values for each state ---------- */
        for (int s = 0; s < STATES; ++s) {
            for (int r = 0; r < 7; ++r)
                val[s][r] = (s >> (2 * r)) & 3;
        }

        /* ---------- pre‑compute left neighbour counts ---------- */
        uint8_t leftCnt[STATES][7];
        for (int s = 0; s < STATES; ++s) {
            for (int r = 0; r < 7; ++r)
                leftCnt[s][r] = (val[s][r] ? 1 : 0)
                              + (r > 0   ? (val[s][r - 1] ? 1 : 0) : 0)
                              + (r < 6   ? (val[s][r + 1] ? 1 : 0) : 0);
        }

        /* ---------- count transitions for each state ---------- */
        size_t totalTrans = 0;
        for (int s = 0; s < STATES; ++s) {
            int cnt = 0;
            for (int nextMask = 0; nextMask < 128; ++nextMask) {
                int ok = 1;
                /* validate current column (after adding right neighbours) */
                for (int r = 0; r < 7 && ok; ++r) {
                    if (!val[s][r]) continue;
                    int partial = (val[s][r] == 1) ? 1
                                    : (val[s][r] == 2) ? 2 : 0;
                    int right = ((nextMask >> r) & 1)
                              + (r > 0   ? ((nextMask >> (r - 1)) & 1) : 0)
                              + (r < 6   ? ((nextMask >> (r + 1)) & 1) : 0);
                    int total = partial + right;
                    if (total != 1 && total != 2) ok = 0;
                }
                if (!ok) continue;
                /* check left side for next column (must not exceed 2 neighbours) */
                for (int r = 0; r < 7 && ok; ++r) {
                    if (nextMask & (1 << r)) {
                        if (leftCnt[s][r] > 2) ok = 0;
                    }
                }
                if (ok) ++cnt;
            }
            transCnt[s] = cnt;
            totalTrans += cnt;
        }

        /* ---------- compute offsets and reserve flat array ---------- */
        offsetST[0] = 0;
        for (int s = 0; s < STATES; ++s)
            offsetST[s + 1] = offsetST[s] + transCnt[s];
        transFlat = (uint16_t *)malloc(totalTrans * sizeof(uint16_t));
        if (!transFlat) return 0; /* out of memory; unlikely in this context */

        /* ---------- fill transitions ----------
           The transition from state s to nextState is inserted into
           transFlat[offsetST[s] .. offsetST[s+1]-1]. -------- */
        for (int s = 0; s < STATES; ++s) {
            size_t idx = offsetST[s];
            for (int nextMask = 0; nextMask < 128; ++nextMask) {
                int ok = 1;
                for (int r = 0; r < 7 && ok; ++r) {
                    if (!val[s][r]) continue;
                    int partial = (val[s][r] == 1) ? 1
                                    : (val[s][r] == 2) ? 2 : 0;
                    int right = ((nextMask >> r) & 1)
                              + (r > 0   ? ((nextMask >> (r - 1)) & 1) : 0)
                              + (r < 6   ? ((nextMask >> (r + 1)) & 1) : 0);
                    int total = partial + right;
                    if (total != 1 && total != 2) ok = 0;
                }
                if (!ok) continue;
                for (int r = 0; r < 7 && ok; ++r) {
                    if (nextMask & (1 << r)) {
                        if (leftCnt[s][r] > 2) ok = 0;
                    }
                }
                if (!ok) continue;
                /* build next state */
                uint16_t nextState = 0;
                for (int r = 0; r < 7; ++r) {
                    if (nextMask & (1 << r)) {
                        int lc = leftCnt[s][r];
                        int nv = (lc == 0) ? 3 : (lc == 1) ? 1 : 2;
                        nextState |= ((uint16_t)nv << (2 * r));
                    }
                }
                transFlat[idx++] = nextState;
            }
        }

        /* ---------- final‑column validity check ---------- */
        for (int s = 0; s < STATES; ++s) {
            int ok = 1;
            for (int r = 0; r < 7 && ok; ++r)
                if (val[s][r] == 3)   /* partial count 0 not allowed at final column */
                    ok = 0;
            validEnd[s] = ok;
        }

        initialized = 1;
    }

    /* ---------- DP arrays ---------- */
    seq_t *dp      = (seq_t *)calloc(STATES, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(STATES, sizeof(seq_t));

    /* ---------- base states (first column) ---------- */
    for (int mask = 0; mask < 128; ++mask) {
        uint16_t state = 0;
        for (int r = 0; r < 7; ++r)
            if (mask & (1 << r))
                state |= ((uint16_t)3 << (2 * r));   /* partial = 0 */
        dp[state] += (seq_t)1;
    }

    /* ---------- special case n==1 ---------- */
    if (n == 1) {
        seq_t ans = 0;
        for (int s = 0; s < STATES; ++s)
            if (validEnd[s])
                ans += dp[s];
        free(dp); free(dp_next);
        return ans;
    }

    /* ---------- perform n-1 transitions ---------- */
    for (idx_t step = 2; step <= n; ++step) {
        memset(dp_next, 0, STATES * sizeof(seq_t));
        for (int s = 0; s < STATES; ++s) {
            if (!dp[s]) continue;
            size_t start = offsetST[s];
            size_t end   = offsetST[s + 1];
            for (size_t idx = start; idx < end; ++idx) {
                uint16_t nxt = transFlat[idx];
                dp_next[nxt] += dp[s];
            }
        }
        seq_t *tmp = dp; dp = dp_next; dp_next = tmp;
    }

    /* ---------- sum over valid final states ---------- */
    seq_t ans = 0;
    for (int s = 0; s < STATES; ++s)
        if (validEnd[s])
            ans += dp[s];

    free(dp); free(dp_next);
    return ans;
}
