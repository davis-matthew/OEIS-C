/*
 * A278090  Number of nX4 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,0) (-1,1) (0,-1) (0,1) or
 *          (1,0), with upper left element zero.
 * https://oeis.org/A278090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with upper-left element zero
 *            such that every element has at least one equal neighbour and one
 *            different neighbour among offsets (-1,0), (-1,1), (0,-1), (0,1),
 *            (1,0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

static const int ALL = 0xF;                /* 4‑bit mask 0b1111 */

/* Pre‑computed masks for a new row: for each (prev,row) pair
   cur_eq[pb][cb]  – positions where the new row element has an equal neighbour
   cur_neq[pb][cb] – positions where the new row element has a different neighbour */
static uint8_t cur_eq[16][16];
static uint8_t cur_neq[16][16];
static bool pre_done = false;

/* Initialise the pre‑computed tables once */
static void init_pre(void) {
    if (pre_done) return;
    for (int pb = 0; pb < 16; ++pb) {
        for (int cb = 0; cb < 16; ++cb) {
            uint8_t eq = 0, neq = 0;
            for (int j = 0; j < 4; ++j) {
                int v  = (cb >> j) & 1;

                /* up neighbour */
                int up = (pb >> j) & 1;
                if (up == v) eq |= 1 << j; else neq |= 1 << j;

                /* up‑right neighbour */
                if (j < 3) {
                    int upr = (pb >> (j + 1)) & 1;
                    if (upr == v) eq |= 1 << j; else neq |= 1 << j;
                }

                /* left neighbour */
                if (j > 0) {
                    int left = (cb >> (j - 1)) & 1;
                    if (left == v) eq |= 1 << j; else neq |= 1 << j;
                }

                /* right neighbour */
                if (j < 3) {
                    int right = (cb >> (j + 1)) & 1;
                    if (right == v) eq |= 1 << j; else neq |= 1 << j;
                }
            }
            cur_eq[pb][cb]  = eq;
            cur_neq[pb][cb] = neq;
        }
    }
    pre_done = true;
}

/* Main routine */
seq_t a(idx_t n) {
    init_pre();

    /* DP arrays and active state lists */
    static seq_t dp[2][4096];
    static int active[2][4096];
    static int active_cnt[2];
    static bool vis[4096];

    int cur = 0, next = 1;
    memset(dp[cur], 0, sizeof(dp[cur]));
    active_cnt[cur] = 0;
    memset(vis, 0, sizeof(vis));

    /* Initialise with first row (upper‑left element fixed to 0) */
    for (int bits = 0; bits < 16; ++bits) {
        if ((bits & 1) != 0) continue;           /* upper‑left (col 0) must be 0 */
        uint8_t eq = 0, neq = 0;
        for (int j = 0; j < 4; ++j) {
            int v = (bits >> j) & 1;
            /* left neighbour */
            if (j > 0) {
                int left = (bits >> (j - 1)) & 1;
                if (left == v) eq  |= 1 << j; else neq |= 1 << j;
            }
            /* right neighbour */
            if (j < 3) {
                int right = (bits >> (j + 1)) & 1;
                if (right == v) eq  |= 1 << j; else neq |= 1 << j;
            }
        }
        int idx = (bits << 8) | (eq << 4) | neq;
        dp[cur][idx] = 1;
        active[cur][active_cnt[cur]++] = idx;
    }

    /* Process remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        /* Reset next DP structures */
        memset(vis, 0, sizeof(vis));
        active_cnt[next] = 0;

        for (int ai = 0; ai < active_cnt[cur]; ++ai) {
            int idx = active[cur][ai];
            seq_t cnt = dp[cur][idx];
            if (cnt == 0) continue;   /* safety */

            int pb   = idx >> 8;
            int peq  = (idx >> 4) & 0xF;
            int pneq = idx & 0xF;

            for (int cb = 0; cb < 16; ++cb) {
                /* Compute masks added by the down neighbour */
                uint8_t eq_add = (~(pb ^ cb)) & 0xF;   /* bits where pb==cb */
                uint8_t neq_add = (pb ^ cb);          /* bits where pb!=cb */

                int peq_f = peq | eq_add;
                int pneq_f = pneq | neq_add;

                /* The previous row must now have both equal and unequal neighbours */
                if ((peq_f & ALL) != ALL || (pneq_f & ALL) != ALL)
                    continue;

                /* New row partial masks from left/right and up/up‑right */
                uint8_t n_eq = cur_eq[pb][cb];
                uint8_t n_neq = cur_neq[pb][cb];

                int n_idx = (cb << 8) | (n_eq << 4) | n_neq;

                if (!vis[n_idx]) {
                    vis[n_idx] = true;
                    active[next][active_cnt[next]++] = n_idx;
                    dp[next][n_idx] = cnt;
                } else {
                    dp[next][n_idx] += cnt;
                }
            }
        }
        /* Swap current and next */
        int tmp = cur; cur = next; next = tmp;
    }

    /* Final answer: sum over states where last row satisfies the condition */
    seq_t result = 0;
    for (int ai = 0; ai < active_cnt[cur]; ++ai) {
        int idx = active[cur][ai];
        int peq  = (idx >> 4) & 0xF;
        int pneq = idx & 0xF;
        if (peq == ALL && pneq == ALL)
            result += dp[cur][idx];
    }
    return result;
}
