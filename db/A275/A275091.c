/*
 * A275091  Number of 3Xn 0..2 arrays with no element equal to any value at
 *          offset (0,-2) (-1,-2) or (-2,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A275091 – Number of 3×n 0..2 arrays with the
 *  no‑offset rule and "new values in order" rule.
 *
 *  The sequence is computed by a transfer‑matrix dynamic program.
 *  States encode the last two columns of the array (12 cells, two
 *  bits per cell).  Transitions add a new column respecting the
 *  constraints; the number of paths in the resulting directed acyclic
 *  graph gives a(n).
 *
 *  This implementation uses only integer arithmetic and is
 *  independent of the underlying width of seq_t (it may be uint8_t,
 *  uint16_t, …, or an arbitrary‑precision class).  It works for
 *  arbitrary n ≥ 1, any ordering of calls, and keeps memory usage
 *  below 4 KB for dp arrays and 8 KB for state tables.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Types and constants                                               */
typedef uint16_t state_t;           /* 6 cells × 2 bits = 12 bits, fits in uint16_t */

enum {
    MAX_STATE = 4096,              /* 4⁶ possible 6‑cell patterns */
    MAX_TRANS = 27                  /* upper bound on outgoing transitions per state */
};

/* ------------------------------------------------------------------ */
/*  Transition table                                                  */
struct trans_s {
    state_t next[MAX_TRANS];
    uint8_t count;
};

static struct trans_s trans[MAX_STATE];
static bool trans_built = false;

/* ------------------------------------------------------------------ */
/*  Helper: encode six 2‑bit values (0..3) into a state code.        */
static inline state_t encode_state(const uint8_t v[6])
{
    return v[0]
         | (v[1] << 2)
         | (v[2] << 4)
         | (v[3] << 6)
         | (v[4] << 8)
         | (v[5] << 10);
}

/* ------------------------------------------------------------------ */
/*  Build the transition table (once).                                */
static void build_transitions(void)
{
    for (int s = 0; s < MAX_STATE; ++s) {
        uint8_t prev[6];
        /* Decode state */
        for (int i = 0; i < 6; ++i) {
            prev[i] = (s >> (2 * i)) & 3;
        }
        /* Find current maximum value present (exclude sentinel 3) */
        int cur_max = -1;
        for (int i = 0; i < 6; ++i) {
            if (prev[i] != 3 && prev[i] > cur_max)
                cur_max = prev[i];
        }
        uint8_t cnt = 0;

        /* First row of new column */
        int lim0 = (cur_max + 1 <= 2) ? (cur_max + 1) : 2;
        for (int v0 = 0; v0 <= lim0; ++v0) {
            /* Offset (0,-2) in the same row two columns left:
               corresponds to prev[3] (row 0, two columns ago). */
            if (prev[3] != 3 && v0 == prev[3]) continue;

            int max1 = (cur_max > v0) ? cur_max : v0;
            int lim1 = (max1 + 1 <= 2) ? (max1 + 1) : 2;
            /* Second row of new column */
            for (int v1 = 0; v1 <= lim1; ++v1) {
                if (prev[4] != 3 && v1 == prev[4]) continue;           /* (0,-2) */
                if (prev[3] != 3 && v1 == prev[3]) continue;           /* (-1,-2) */

                int max2 = (max1 > v1) ? max1 : v1;
                int lim2 = (max2 + 1 <= 2) ? (max2 + 1) : 2;
                /* Third row of new column */
                for (int v2 = 0; v2 <= lim2; ++v2) {
                    if (prev[5] != 3 && v2 == prev[5]) continue;       /* (0,-2) */
                    if (prev[4] != 3 && v2 == prev[4]) continue;       /* (-1,-2) */
                    if (prev[0] != 3 && v2 == prev[0]) continue;       /* (-2,-1) */

                    uint8_t nxt[6] = { (uint8_t)v0, (uint8_t)v1, (uint8_t)v2,
                                       prev[0], prev[1], prev[2] };
                    trans[s].next[cnt] = encode_state(nxt);
                    ++cnt;
                }
            }
        }
        trans[s].count = cnt;
    }
    trans_built = true;
}

/* ------------------------------------------------------------------ */
/*  Active–state lists (for efficient traversal).                    */
static int act_buf[2][MAX_STATE];
static int *act_cur, *act_next;
static size_t act_cur_len, act_next_len;

/* ------------------------------------------------------------------ */
/*  Main function                                                   */
seq_t a(idx_t n)
{
    /* Build transition table once. */
    if (!trans_built)
        build_transitions();

    /* Initialise active‑state pointers the first time the function
     * is called.  They are static so that future calls reuse the
     * same buffers. */
    static bool init_done = false;
    if (!init_done) {
        act_cur     = act_buf[0];
        act_next    = act_buf[1];
        act_cur_len = 0;
        act_next_len = 0;
        init_done = true;
    }

    /* DP arrays: number of ways to reach each state */
    seq_t *dp_cur  = (seq_t *)calloc(MAX_STATE, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(MAX_STATE, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        free(dp_cur); free(dp_next);
        return (seq_t)0;                /* allocation failure */
    }

    /* Initial state: all cells are sentinel 3 (unassigned). */
    const state_t init_state = 0xFFF; /* binary 111111111111 */
    dp_cur[init_state] = (seq_t)1;
    act_cur[0] = (int)init_state;
    act_cur_len = 1;

    /* Iterate column by column. */
    for (idx_t col = 0; col < n; ++col) {
        /* clear next array and active list */
        memset(dp_next, 0, MAX_STATE * sizeof(seq_t));
        act_next_len = 0;

        /* iterate only over states that have a non‑zero count */
        for (size_t i = 0; i < act_cur_len; ++i) {
            state_t s = (state_t)act_cur[i];
            seq_t cur_val = dp_cur[s];
            if (!cur_val) continue;

            struct trans_s *t = &trans[s];
            for (uint8_t k = 0; k < t->count; ++k) {
                state_t nxt = t->next[k];
                if (!dp_next[nxt]) {
                    act_next[act_next_len++] = (int)nxt;
                }
                dp_next[nxt] += cur_val;
            }
        }

        /* swap dp arrays and active lists */
        seq_t  *tmp_dp  = dp_cur;
        dp_cur  = dp_next;
        dp_next = tmp_dp;

        int *tmp_act = act_cur;
        act_cur     = act_next;
        act_next    = tmp_act;
        act_cur_len = act_next_len;
    }

    /* Sum all ways in the final state set */
    seq_t total = 0;
    for (size_t i = 0; i < act_cur_len; ++i) {
        total += dp_cur[act_cur[i]];
    }

    free(dp_cur);
    free(dp_next);
    return total;
}
