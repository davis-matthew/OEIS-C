/*
 * A282836  Number of nX6 0..1 arrays with no 1 equal to more than one of its
 *          king-move neighbors, with the exception of exactly two elements.
 * https://oeis.org/A282836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A282836
    Count of n×6 binary arrays in which no 1 has more than one king‑move
    adjacent 1, except exactly two elements.
    This implementation uses dynamic programming over row states.
    It keeps the number of violations seen so far and discards
    partial arrays that would exceed two violations.
*/

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Total number of row‑states: 4^6 = 4096. */
    enum { NS = 4096 };

    /* Static tables are initialised once. */
    static int init_flag = 0;

    /* For a state we store: column mask, digit array (0–2 per column),
       number of violating cells in the state, transitions to the next
       state, and the increase in violations from the previous row. */
    static uint8_t state_mask[NS];
    static uint8_t state_digit[NS][6];
    static uint8_t state_viol[NS];
    static int next_state[NS][64];     /* to a state ID for a given mask */
    static uint8_t delta_inc[NS][64];  /* violations added when finishing a row */
    static int id_of[64][729];         /* lookup id by (mask, base) */
    static uint8_t pow3[6];
    static uint8_t neighbor_mask[6];

    if (!init_flag) {
        /* powers of 3 for base encoding */
        pow3[0] = 1;
        for (int i = 1; i < 6; ++i)
            pow3[i] = pow3[i - 1] * 3;

        /* horizontal neighbour masks within one row (self, left, right) */
        for (int c = 0; c < 6; ++c) {
            uint8_t m = 0;
            for (int d = -1; d <= 1; ++d) {
                int nc = c + d;
                if (nc >= 0 && nc < 6)
                    m |= (1U << nc);
            }
            neighbor_mask[c] = m;
        }

        /* initialise id_of with -1 */
        for (int m = 0; m < 64; ++m)
            for (int b = 0; b < 729; ++b)
                id_of[m][b] = -1;

        /* enumerate all admissible states */
        int sid = 0;
        for (int m = 0; m < 64; ++m) {
            for (int base = 0; base < 729; ++base) {   /* 3^6 */
                int x = base;
                bool ok = true;
                for (int c = 0; c < 6; ++c) {
                    uint8_t dgt = x % 3;
                    x /= 3;
                    if (((m >> c) & 1U) == 0) {
                        if (dgt != 0) { ok = false; break; }
                    }
                    state_digit[sid][c] = dgt;
                }
                if (!ok) continue;
                state_mask[sid] = (uint8_t)m;
                id_of[m][base] = sid;
                ++sid;
            }
        }

        /* compute violations for each state */
        for (int id = 0; id < sid; ++id) {
            uint8_t v = 0;
            for (int c = 0; c < 6; ++c)
                if ((state_mask[id] >> c) & 1U)
                    if (state_digit[id][c] == 2) ++v;
            state_viol[id] = v;
        }

        /* precompute transitions */
        for (int pid = 0; pid < sid; ++pid) {
            int pmask = state_mask[pid];
            for (int cmask = 0; cmask < 64; ++cmask) {
                /* violations added by finishing the previous row */
                int addv = 0;
                for (int c = 0; c < 6; ++c) {
                    if ((pmask >> c) & 1U) {
                        int prev_cnt = state_digit[pid][c];
                        int add = __builtin_popcount((unsigned)(cmask & neighbor_mask[c]));
                        if (prev_cnt + add > 1)
                            ++addv;
                    }
                }
                delta_inc[pid][cmask] = (uint8_t)addv;

                /* new state for the current row */
                uint8_t nat_dig[6] = {0};
                for (int c = 0; c < 6; ++c) {
                    if ((cmask >> c) & 1U) {
                        int vert = __builtin_popcount((unsigned)(pmask & neighbor_mask[c]));
                        int leftright = __builtin_popcount((unsigned)(cmask & neighbor_mask[c])) - 1;
                        int val = vert + leftright;
                        if (val > 2) val = 2;
                        nat_dig[c] = (uint8_t)val;
                    }
                }
                int base = 0;
                for (int c = 0; c < 6; ++c)
                    base += nat_dig[c] * pow3[c];
                int nid = id_of[cmask][base];
                next_state[pid][cmask] = nid;
            }
        }

        init_flag = 1;
    }

    /* --- dynamic programming over rows ------------------------------------- */
    static seq_t dp[2][NS][3];
    int cur = 0, next = 1;

    /* clear dp arrays */
    memset(dp, 0, sizeof(dp));

    /* active state lists */
    static int active_cur[NS];
    static int active_next[NS];
    static bool in_next[NS];
    int cur_len = 0, next_len = 0;

    /* initial row: for each possible pattern */
    for (int m = 0; m < 64; ++m) {
        uint8_t initdig[6] = {0};
        for (int c = 0; c < 6; ++c) {
            if ((m >> c) & 1U) {
                int within = __builtin_popcount((unsigned)(m & neighbor_mask[c])) - 1;
                if (within > 2) within = 2;
                initdig[c] = (uint8_t)within;
            }
        }
        int base = 0;
        for (int c = 0; c < 6; ++c)
            base += initdig[c] * pow3[c];
        int sid = id_of[m][base];
        if (sid < 0) continue;
        dp[cur][sid][0] = 1;
        active_cur[cur_len++] = sid;
    }

    /* process remaining rows */
    for (idx_t r = 1; r < n; ++r) {
        /* clear next buffer */
        for (int i = 0; i < NS; ++i)
            for (int k = 0; k < 3; ++k)
                dp[next][i][k] = 0;
        memset(in_next, 0, sizeof(in_next));
        next_len = 0;

        /* transition from active states */
        for (int ai = 0; ai < cur_len; ++ai) {
            int pid = active_cur[ai];
            seq_t v0 = dp[cur][pid][0];
            seq_t v1 = dp[cur][pid][1];
            seq_t v2 = dp[cur][pid][2];
            if (v0 == 0 && v1 == 0 && v2 == 0)
                continue;
            for (int cmask = 0; cmask < 64; ++cmask) {
                int nid = next_state[pid][cmask];
                int inc = delta_inc[pid][cmask];
                if (inc > 2) continue;

                /* propagate all violation counts */
                if (v0 != 0) {
                    int nk = inc;
                    if (nk <= 2)
                        dp[next][nid][nk] += v0;
                }
                if (v1 != 0) {
                    int nk = inc + 1;
                    if (nk <= 2)
                        dp[next][nid][nk] += v1;
                }
                if (v2 != 0) {
                    int nk = inc + 2;
                    if (nk <= 2)
                        dp[next][nid][nk] += v2;
                }

                /* record state for next active list */
                if (!in_next[nid]) {
                    active_next[next_len++] = nid;
                    in_next[nid] = true;
                }
            }
        }

        /* swap buffers and active lists */
        cur ^= 1;
        next ^= 1;
        memcpy(active_cur, active_next, sizeof(active_next));
        cur_len = next_len;
    }

    /* finalize last row: count only arrays with total 2 violations */
    seq_t ans = 0;
    for (int ai = 0; ai < cur_len; ++ai) {
        int sid = active_cur[ai];
        for (int k = 0; k < 3; ++k) {
            seq_t val = dp[cur][sid][k];
            if (val == 0) continue;
            int total = k + state_viol[sid];
            if (total == 2)
                ans += val;
        }
    }
    return ans;
}
