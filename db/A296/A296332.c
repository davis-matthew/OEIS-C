/*
 * A296332  Number of nX5 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A296332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays such that each 1 has 0 or 2
 *            adjacent 1s horizontally, vertically or anti‑diagonally
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define N_MASK   32          /* 2^5  */
#define N_COL    5
#define N_STATE  (N_MASK * N_MASK)

/* static storage for precomputed tables */
static uint8_t horiz_deg[N_MASK][N_COL];
static uint8_t next_cnt[N_STATE];
static uint8_t next_mask[N_STATE][N_MASK];

/* initialise tables on first call */
static void init_tables(void)
{
    static bool ready = false;
    if (ready) return;
    /* horizontal degree for every column of a mask */
    for (int mask = 0; mask < N_MASK; ++mask) {
        for (int i = 0; i < N_COL; ++i) {
            if ((mask >> i) & 1) {
                int d = 0;
                if (i > 0      && ((mask >> (i-1)) & 1)) d++;
                if (i + 1 < N_COL && ((mask >> (i+1)) & 1)) d++;
                horiz_deg[mask][i] = (uint8_t)d;
            } else {
                horiz_deg[mask][i] = 0;
            }
        }
    }
    /* precompute valid transitions between (prevMask, curMask) and nextMask */
    for (int prevMask = 0; prevMask < N_MASK; ++prevMask) {
        for (int curMask = 0; curMask < N_MASK; ++curMask) {
            int state = prevMask * N_MASK + curMask;
            uint8_t cnt = 0;
            for (int nxtMask = 0; nxtMask < N_MASK; ++nxtMask) {
                bool ok = true;
                for (int i = 0; i < N_COL && ok; ++i) {
                    if (!((curMask >> i) & 1)) continue;
                    int deg = horiz_deg[curMask][i];
                    /* contributions from the row above (prevMask) */
                    if ((prevMask >> i)          & 1) deg++;
                    if (i + 1 < N_COL
                        && ((prevMask >> (i+1)) & 1)) deg++;
                    /* contributions from the row below (nxtMask) */
                    if ((nxtMask >> i)          & 1) deg++;
                    if (i > 0
                        && ((nxtMask >> (i-1)) & 1)) deg++;
                    if (deg != 0 && deg != 2) { ok = false; break; }
                }
                if (ok) {
                    next_mask[state][cnt++] = (uint8_t)nxtMask;
                }
            }
            next_cnt[state] = cnt;
        }
    }
    ready = true;
}

/* final row validity when there is no following row (no below contributions) */
static inline bool final_row_ok(int prevMask, int curMask)
{
    for (int i = 0; i < N_COL; ++i) {
        if (!((curMask >> i) & 1)) continue;
        int deg = horiz_deg[curMask][i];
        if ((prevMask >> i)          & 1) deg++;
        if (i + 1 < N_COL
            && ((prevMask >> (i+1)) & 1)) deg++;
        /* no below neighbours */
        if (deg != 0 && deg != 2) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    init_tables();

    static seq_t dp_cur[N_STATE];
    static seq_t dp_next[N_STATE];

    /* initialise with the first row (prevMask = 0) */
    for (int i = 0; i < N_STATE; ++i) dp_cur[i] = (seq_t)0;
    for (int firstMask = 0; firstMask < N_MASK; ++firstMask) {
        dp_cur[firstMask] = (seq_t)1;           /* state id = 0 * N_MASK + firstMask */
    }

    /* process remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < N_STATE; ++i) dp_next[i] = (seq_t)0;
        for (int state = 0; state < N_STATE; ++state) {
            seq_t ways = dp_cur[state];
            if (ways == (seq_t)0) continue;
            int curMask = state % N_MASK;
            uint8_t cnt = next_cnt[state];
            for (int k = 0; k < cnt; ++k) {
                int nxtMask  = next_mask[state][k];
                int nxtState = curMask * N_MASK + nxtMask;
                dp_next[nxtState] = dp_next[nxtState] + ways;
            }
        }
        for (int i = 0; i < N_STATE; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum over states where the last row is valid (no below contributions) */
    seq_t total = (seq_t)0;
    for (int state = 0; state < N_STATE; ++state) {
        seq_t ways = dp_cur[state];
        if (ways == (seq_t)0) continue;
        int prevMask = state / N_MASK;
        int lastMask = state % N_MASK;
        if (final_row_ok(prevMask, lastMask)) {
            total = total + ways;
        }
    }
    return total;
}
