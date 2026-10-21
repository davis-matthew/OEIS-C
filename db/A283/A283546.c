/*
 * A283546  Number of 5Xn 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) evaluated by a DP that tracks the last two columns. The
 *            state space is (prev2,prev1) where each column is a 5‑bit
 *            pattern. For a transition prev2,prev1 → cur, we require that
 *            every 1 in prev1 has at most one neighbour among left (prev2),
 *            right (cur), up‑left (prev2), down‑right (cur), up‑rig...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* Helpers – always valid for all supported seq_t types */

static inline int
valid_transition(unsigned char prev2, unsigned char prev1,
                 unsigned char cur)
{
    /* Check all rows that contain a 1 in prev1 */
    for (int r = 0; r < 5; ++r) {
        if ((prev1 >> r) & 1) {
            int cnt = 0;
            if ((prev2 >> r) & 1)             ++cnt;         /* left */
            if ((cur   >> r) & 1)             ++cnt;         /* right */
            if (r > 0 && (prev2 >> (r - 1)) & 1) ++cnt;      /* up-left */
            if (r < 4 && (cur   >> (r + 1)) & 1) ++cnt;      /* down-right */
            if (r > 0 && (cur   >> (r - 1)) & 1) ++cnt;      /* up-right */
            if (r < 4 && (prev2 >> (r + 1)) & 1) ++cnt;      /* down-left */
            if (cnt > 1) return 0;
        }
    }
    return 1;
}

/* --------------------------------------------------------------------- */
/* Transition tables (initialised once per program run) */

#define STATE_COUNT 32
#define TOTAL_STATES (STATE_COUNT * STATE_COUNT)   /* 1024 */

static unsigned char   trans_cnt[TOTAL_STATES];          /* #next states */
static unsigned short  trans_next[TOTAL_STATES][STATE_COUNT]; /* next states */
static int             trans_inited = 0;

static void
init_transitions(void)
{
    if (trans_inited) return;
    for (int p2 = 0; p2 < STATE_COUNT; ++p2) {
        for (int p1 = 0; p1 < STATE_COUNT; ++p1) {
            int idx = p2 * STATE_COUNT + p1;
            int cnt = 0;
            for (int cur = 0; cur < STATE_COUNT; ++cur) {
                if (valid_transition((unsigned char)p2,
                                     (unsigned char)p1,
                                     (unsigned char)cur)) {
                    trans_next[idx][cnt] = (unsigned short)(p1 * STATE_COUNT + cur);
                    ++cnt;
                }
            }
            trans_cnt[idx] = (unsigned char)cnt;
        }
    }
    trans_inited = 1;
}

/* --------------------------------------------------------------------- */
/* Main function required by the exercise */

seq_t
a(idx_t n)
{
    init_transitions();

    if (n == 0) return 0;             /* undefined but harmless */

    seq_t dp[TOTAL_STATES]  = {0};
    seq_t ndp[TOTAL_STATES] = {0};

    /* Initialise: column 1, prev2=0 (sentinel), prev1 varies */
    for (int pat = 0; pat < STATE_COUNT; ++pat) {
        dp[pat] = (seq_t)1;           /* state (0,pat) */
    }

    /* Process columns 2 … n */
    for (idx_t col = 2; col <= n; ++col) {
        for (int i = 0; i < TOTAL_STATES; ++i) ndp[i] = 0;

        for (int prev = 0; prev < TOTAL_STATES; ++prev) {
            seq_t val = dp[prev];
            if (!val) continue;
            int cnt = trans_cnt[prev];
            for (int t = 0; t < cnt; ++t) {
                unsigned short nxt = trans_next[prev][t];
                ndp[nxt] = ndp[nxt] + val;
            }
        }

        /* copy back for next iteration */
        for (int i = 0; i < TOTAL_STATES; ++i) dp[i] = ndp[i];
    }

    /* Final column check: the right neighbour is 0 */
    seq_t total = 0;
    for (int idx = 0; idx < TOTAL_STATES; ++idx) {
        int p2 = idx / STATE_COUNT;
        int p1 = idx % STATE_COUNT;
        if (valid_transition((unsigned char)p2,
                             (unsigned char)p1,
                             (unsigned char)0)) {
            total = total + dp[idx];
        }
    }

    return total;
}
