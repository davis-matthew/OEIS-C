/*
 * A282861  Number of nX7 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A282861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays in which no 1 has more than
 *            one of its horizontal, vertical and antidiagonal neighbors. The
 *            DP processes the grid row by row
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define WIDTH 7
#define MASK_ALL ((1u << WIDTH) - 1)
#define STATECOUNT ((1 << WIDTH) * (1 << WIDTH))

/* Transition table structures */
typedef struct {
    uint8_t mask;   /* next row mask */
    uint8_t sat;    /* next row saturated mask */
} Trans;

/* Maximum number of transition entries (tuned empirically). */
#define MAX_TRANS 5000000

static Trans  trans_buf[MAX_TRANS];
static int   trans_start[STATECOUNT];
static int   trans_len  [STATECOUNT];
static bool  trans_prepared = false;

/* Precompute all admissible transitions between states. */
static void prepare_transitions(void)
{
    if (trans_prepared) return;
    int ptr = 0;

    for (int prevMask = 0; prevMask <= MASK_ALL; ++prevMask) {
        for (int prevSat = 0; prevSat <= MASK_ALL; ++prevSat) {
            int stateIdx = (prevMask << WIDTH) | prevSat;
            trans_start[stateIdx] = ptr;
            int used = 0;

            uint8_t unsatMask = prevMask & (~prevSat) & MASK_ALL;

            for (int curMask = 0; curMask <= MASK_ALL; ++curMask) {
                uint8_t m = (uint8_t)curMask;

                /* 1. No three consecutive 1s in the same row */
                if ((m & (m >> 1) & (m >> 2)) != 0) continue;

                /* 2. Horizontal saturation within the current row */
                uint8_t left  = (m << 1) & MASK_ALL;
                uint8_t right = m >> 1;
                uint8_t satHor = m & (left | right);

                /* 3. Saturated cells of previous row cannot receive
                   vertical or antidiagonal neighbors now. */
                if (((prevSat | (prevSat >> 1)) & m) != 0) continue;

                /* 4. Unsatisfied previous cells cannot have both vertical
                   and diagonal neighbors to the same current cell. */
                bool conflict = false;
                for (int p = 0; p < WIDTH; ++p) {
                    if ((unsatMask >> p) & 1) {
                        bool v = (m >> p) & 1;
                        bool d = (p > 0) ? ((m >> (p - 1)) & 1) : false;
                        if (v && d) { conflict = true; break; }
                    }
                }
                if (conflict) continue;

                /* 5. Two unsatisfied cells cannot match the same
                   current cell (cross conflict). */
                if (((unsatMask & (unsatMask << 1)) & m) != 0) continue;

                /* 6. Cells matched vertically or antidiagonally. */
                uint8_t vertMask = unsatMask & m;
                uint8_t diagMask = (unsatMask >> 1) & m;   /* unsat p -> current p-1 */

                /* 7. The matched cells cannot also have a horizontal neighbor. */
                if (satHor & (vertMask | diagMask)) continue;

                uint8_t satCurr = satHor | vertMask | diagMask;

                /* Store the transition */
                if (ptr >= MAX_TRANS) {
                    /* In practice this should never happen. */
                    /* If it does, abort precomputation. */
                    trans_prepared = false;
                    return;
                }
                trans_buf[ptr].mask = (uint8_t)curMask;
                trans_buf[ptr].sat  = satCurr;
                ++ptr;
                ++used;
            }
            trans_len[stateIdx] = used;
        }
    }

    trans_prepared = true;
}

/* Main sequence function.
   Called by the testing harness with an arbitrary seq_t type. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n < 1 */
    prepare_transitions();

    static seq_t dp1[1 << WIDTH][1 << WIDTH];
    static seq_t dp2[1 << WIDTH][1 << WIDTH];
    seq_t (*cur)[1 << WIDTH] = dp1;
    seq_t (*nxt)[1 << WIDTH] = dp2;

    memset(cur, 0, sizeof(dp1));

    /* Initial row (row 1) */
    for (int mask = 0; mask <= MASK_ALL; ++mask) {
        uint8_t m = (uint8_t)mask;
        if ((m & (m >> 1) & (m >> 2)) != 0) continue;   /* forbid 111 */
        uint8_t left  = (m << 1) & MASK_ALL;
        uint8_t right = m >> 1;
        uint8_t satHor = m & (left | right);
        cur[mask][satHor] = 1;
    }

    /* Process remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        memset(nxt, 0, sizeof(dp2));
        for (int prevMask = 0; prevMask <= MASK_ALL; ++prevMask) {
            for (int prevSat = 0; prevSat <= MASK_ALL; ++prevSat) {
                seq_t val = cur[prevMask][prevSat];
                if (val == 0) continue;
                int stateIdx = (prevMask << WIDTH) | prevSat;
                int start    = trans_start[stateIdx];
                int count    = trans_len[stateIdx];
                for (int i = 0; i < count; ++i) {
                    const Trans *e = &trans_buf[start + i];
                    nxt[e->mask][e->sat] += val;
                }
            }
        }
        /* Swap for next iteration */
        seq_t (*tmp)[1 << WIDTH] = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all states for the last row */
    seq_t ans = 0;
    for (int mask = 0; mask <= MASK_ALL; ++mask)
        for (int sat = 0; sat <= MASK_ALL; ++sat)
            ans += cur[mask][sat];

    return ans;
}
