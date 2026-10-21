/*
 * A296686  Number of nX6 0..1 arrays with each 1 adjacent to 1, 2 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296686:
   Number of n×6 0..1 arrays where every 1 has 1–3 king‑move neighboring 1s.
   Computed by a 2‑row transfer‑matrix dynamic programming. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* not used, but safe */

    /* ---------- Precomputation of transitions (once) ---------- */
    static bool initialized = false;
    static uint8_t transCnt[4096];
    static uint8_t transList[4096][64];
    static int neigh_above[6], neigh_below[6], neigh_lr[6];

    if (!initialized) {
        /* neighbor masks */
        for (int i = 0; i < 6; ++i) {
            int lr = 0;
            if (i > 0) lr |= 1 << (i - 1);
            if (i < 5) lr |= 1 << (i + 1);
            neigh_lr[i] = lr;

            int ab = 0;
            if (i > 0) ab |= 1 << (i - 1);
            ab |= 1 << i;
            if (i < 5) ab |= 1 << (i + 1);
            neigh_above[i] = ab;
            neigh_below[i] = ab;
        }

        for (int prev2 = 0; prev2 < 64; ++prev2) {
            for (int prev1 = 0; prev1 < 64; ++prev1) {
                int state = prev2 * 64 + prev1;
                int cnt = 0;
                for (int cur = 0; cur < 64; ++cur) {
                    bool ok = true;
                    for (int i = 0; i < 6; ++i) {
                        if (prev1 & (1 << i)) {
                            int nb = 0;
                            nb += __builtin_popcount((uint64_t)(neigh_above[i] & prev2));
                            nb += __builtin_popcount((uint64_t)(neigh_lr[i] & prev1));
                            nb += __builtin_popcount((uint64_t)(neigh_below[i] & cur));
                            if (nb < 1 || nb > 3) {
                                ok = false;
                                break;
                            }
                        }
                    }
                    if (ok) {
                        transList[state][cnt++] = (uint8_t)cur;
                    }
                }
                transCnt[state] = (uint8_t)cnt;
            }
        }
        initialized = true;
    }

    /* ---------- DP over rows ---------- */
    static seq_t dp[2][4096];
    int curIdx = 0, nxtIdx = 1;
    for (int i = 0; i < 4096; ++i) dp[curIdx][i] = 0;
    dp[curIdx][0] = 1;                     /* state (0,0) */

    for (idx_t r = 0; r < n; ++r) {
        for (int i = 0; i < 4096; ++i) dp[nxtIdx][i] = 0;
        for (int s = 0; s < 4096; ++s) {
            seq_t val = dp[curIdx][s];
            if (val == 0) continue;
            int prev1 = s & 63;
            int cnt = transCnt[s];
            for (int t = 0; t < cnt; ++t) {
                int curMask = transList[s][t];
                int ns = (prev1 << 6) | curMask;   /* new state (prev1, curMask) */
                dp[nxtIdx][ns] += val;
            }
        }
        int tmp = curIdx; curIdx = nxtIdx; nxtIdx = tmp;
    }

    /* ---------- Final row restriction ---------- */
    seq_t result = 0;
    for (int s = 0; s < 4096; ++s) {
        seq_t val = dp[curIdx][s];
        if (val == 0) continue;
        int prev2 = s >> 6;
        int prev1 = s & 63;
        bool ok = true;
        for (int i = 0; i < 6; ++i) {
            if (prev1 & (1 << i)) {
                int nb = 0;
                nb += __builtin_popcount((uint64_t)(neigh_above[i] & prev2));
                nb += __builtin_popcount((uint64_t)(neigh_lr[i] & prev1));
                /* below row is zero */
                if (nb < 1 || nb > 3) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) result += val;
    }
    return result;
}
