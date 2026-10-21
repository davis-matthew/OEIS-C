/*
 * A296574  Number of nX4 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 0, 1 or 3 neighboring 1s.
 * https://oeis.org/A296574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) is the number of n×4 binary arrays in which each 1 has *
 *            0, 1 or 3 neighbours among its horizontal (left/right), *
 *            vertical (up/down), up‑right and down‑left adjacent cells. * *
 *            The implementation uses a DP over the state of the previous row.
 *            * Each state stores: * - mask : 4-bit pattern o...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* maximum number of states: 16 masks × 4⁴ encodings = 625 */
#define MAX_STATES 625

static int stateIndex[1 << 12];          /* key -> state id (enc<<4 | mask) */
static int mask_of[MAX_STATES];          /* state id -> previous row mask (4 bits) */
static uint8_t countsEnc_of[MAX_STATES]; /* state id -> packed neighbour counts */
static int trans_next[MAX_STATES][16];   /* outgoing transitions */
static int trans_count[MAX_STATES];     /* number of successors per state */
static bool initialized = false;

/* Build the state transition graph once. */
static void init_state(void)
{
    if (initialized) return;

    /* initialise all keys to -1 */
    for (int i = 0; i < (1 << 12); ++i) stateIndex[i] = -1;

    int sid = 0;

    /* enumerate all possible states */
    for (int mask = 0; mask < 16; ++mask) {
        for (int enc = 0; enc < 256; ++enc) {
            bool ok = true;
            /* encoded counts must be 0 for zero cells */
            for (int j = 0; j < 4; ++j) {
                if (!(mask & (1 << j))) {
                    if ((enc >> (2 * j)) & 3) { ok = false; break; }
                }
            }
            if (!ok) continue;
            int key = (enc << 4) | mask;
            stateIndex[key] = sid;
            mask_of[sid]      = mask;
            countsEnc_of[sid] = (uint8_t)enc;
            ++sid;
        }
    }

    /* build outgoing transitions for each state */
    for (int s = 0; s < sid; ++s) {
        int prevMask   = mask_of[s];
        uint8_t prevEnc = countsEnc_of[s];
        int prevCnt[4];
        for (int j = 0; j < 4; ++j) prevCnt[j] = (prevEnc >> (2 * j)) & 3;

        int tcnt = 0;
        for (int curMask = 0; curMask < 16; ++curMask) {
            bool ok = true;

            /* finalize all 1s in the previous row using the new row */
            for (int j = 0; j < 4; ++j) if (prevMask & (1 << j)) {
                int cnt = prevCnt[j];
                if (curMask & (1 << j))          ++cnt;          /* down neighbour */
                if (j > 0 && (curMask & (1 << (j - 1)))) ++cnt;    /* down-left neighbour */
                /* down-right is NOT part of the adjacency set */
                if (!(cnt == 0 || cnt == 1 || cnt == 3)) { ok = false; break; }
            }
            if (!ok) continue;

            uint8_t newEnc = 0;

            /* build state for current row (partial counts so far) */
            for (int j = 0; j < 4; ++j) if (curMask & (1 << j)) {
                int cnt = 0;
                /* horizontal neighbours */
                if (j > 0 && (curMask & (1 << (j - 1)))) ++cnt;
                if (j < 3 && (curMask & (1 << (j + 1)))) ++cnt;
                /* vertical and up‑right neighbour from previous row */
                if (prevMask & (1 << j))            ++cnt;          /* up neighbour */
                if (j < 3 && (prevMask & (1 << (j + 1)))) ++cnt;   /* up‑right neighbour */
                if (cnt > 3) { ok = false; break; }               /* exceeds allowed range */
                newEnc |= (cnt << (2 * j));
            }
            if (!ok) continue;

            int nextIdx = stateIndex[(newEnc << 4) | curMask];
            trans_next[s][tcnt++] = nextIdx;
        }
        trans_count[s] = tcnt;
    }

    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but guard */

    init_state();

    static seq_t dp[MAX_STATES];
    static seq_t ndp[MAX_STATES];
    for (int i = 0; i < MAX_STATES; ++i) { dp[i] = 0; ndp[i] = 0; }

    /* first row: no previous row, only horizontal neighbours count */
    for (int mask = 0; mask < 16; ++mask) {
        uint8_t enc = 0;
        for (int j = 0; j < 4; ++j) if (mask & (1 << j)) {
            int cnt = 0;
            if (j > 0 && (mask & (1 << (j - 1)))) ++cnt;
            if (j < 3 && (mask & (1 << (j + 1)))) ++cnt;
            enc |= (cnt << (2 * j));
        }
        int sid = stateIndex[(enc << 4) | mask];
        dp[sid] += (seq_t)1;
    }

    /* process remaining rows */
    if (n > 1) {
        for (idx_t row = 2; row <= n; ++row) {
            for (int i = 0; i < MAX_STATES; ++i) ndp[i] = 0;
            for (int s = 0; s < MAX_STATES; ++s) {
                seq_t ways = dp[s];
                if (!ways) continue;
                for (int k = 0; k < trans_count[s]; ++k) {
                    int nxt = trans_next[s][k];
                    ndp[nxt] += ways;
                }
            }
            /* swap dp and ndp */
            for (int i = 0; i < MAX_STATES; ++i) {
                seq_t tmp = dp[i];
                dp[i] = ndp[i];
                ndp[i] = tmp;
            }
        }
    }

    /* final row must satisfy neighbour rule (no down‑left neighbour) */
    seq_t total = 0;
    for (int s = 0; s < MAX_STATES; ++s) {
        seq_t ways = dp[s];
        if (!ways) continue;
        int mask = mask_of[s];
        uint8_t enc = countsEnc_of[s];
        bool ok = true;
        for (int j = 0; j < 4; ++j) if (mask & (1 << j)) {
            int cnt = (enc >> (2 * j)) & 3;
            if (!(cnt == 0 || cnt == 1 || cnt == 3)) { ok = false; break; }
        }
        if (ok) total += ways;
    }

    return total;
}
