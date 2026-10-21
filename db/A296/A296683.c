/*
 * A296683  Number of n X 3 0..1 arrays with each 1 adjacent to 1, 2 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes number of n×3 binary arrays where each 1 has 1–3 king‑move neighbors. */
#include <stdlib.h>
#include <stdint.h>

/* Transition tables for dynamic programming over rows. */
#define STATE_COUNT   125
#define PATTERN_COUNT 8

static int transition_table[STATE_COUNT][PATTERN_COUNT];
static int final_valid[STATE_COUNT];
static int init_done = 0;

/* Encode a state from three values 0–4 (4 means "no cell"). */
static inline int encode_state(int v0, int v1, int v2)
{
    return v0 * 25 + v1 * 5 + v2;
}

/* Initialise transition table and final‑state validity.  Called once. */
static void init_tables(void)
{
    if (init_done) return;

    /* Pre‑compute partial values and pattern bits for each state. */
    int part[STATE_COUNT][3];
    int patt[STATE_COUNT][3];

    for (int v0 = 0; v0 <= 4; ++v0)
    for (int v1 = 0; v1 <= 4; ++v1)
    for (int v2 = 0; v2 <= 4; ++v2)
    {
        int idx = encode_state(v0, v1, v2);
        part[idx][0] = v0; part[idx][1] = v1; part[idx][2] = v2;
        patt[idx][0] = (v0 != 4) ? 1 : 0;
        patt[idx][1] = (v1 != 4) ? 1 : 0;
        patt[idx][2] = (v2 != 4) ? 1 : 0;
    }

    /* Compute transition table. */
    for (int s = 0; s < STATE_COUNT; ++s)
    for (int pat = 0; pat < PATTERN_COUNT; ++pat)
    {
        int prev[3]   = { part[s][0], part[s][1], part[s][2] };
        int prevMask  = (patt[s][0] << 0) | (patt[s][1] << 1) | (patt[s][2] << 2);

        /* If any 1 from the previous row cannot reach 1–3 neighbors after
           adding this row, the transition is invalid. */
        int valid = 1;
        for (int j = 0; j < 3 && valid; ++j)
        {
            if (prev[j] == 4) continue;          /* was a 0 cell */
            int below = 0;
            for (int k = j - 1; k <= j + 1; ++k)
                if (k >= 0 && k < 3 && (pat & (1 << k))) ++below;
            int fin = prev[j] + below;
            if (fin < 1 || fin > 3) valid = 0;
        }

        if (!valid) { transition_table[s][pat] = -1; continue; }

        /* Build partial counts for this row to be stored as next state. */
        int nextp[3];
        for (int j = 0; j < 3; ++j)
        {
            if ((pat & (1 << j)) == 0) {          /* cell is 0 */
                nextp[j] = 4;
                continue;
            }
            int cnt = 0;
            /* Above neighbors */
            for (int k = j - 1; k <= j + 1; ++k)
                if (k >= 0 && k < 3 && prevMask & (1 << k)) ++cnt;
            /* Left/right within the same row */
            if (j > 0      && (pat & (1 << (j - 1)))) ++cnt;
            if (j < 2      && (pat & (1 << (j + 1)))) ++cnt;
            if (cnt > 3) { valid = 0; break; }
            nextp[j] = cnt;           /* cnt is 0..3 */
        }
        if (!valid) transition_table[s][pat] = -1;
        else transition_table[s][pat] = encode_state(nextp[0], nextp[1], nextp[2]);
    }

    /* Pre‑compute which states are valid as final states
       (no pending cells, and every 1 has at least one neighbor). */
    for (int s = 0; s < STATE_COUNT; ++s)
    {
        int ok = 1;
        for (int j = 0; j < 3; ++j)
        {
            int v = part[s][j];
            if (v != 4 && v == 0) { ok = 0; break; }
        }
        final_valid[s] = ok;
    }

    init_done = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    init_tables();

    static seq_t cur[STATE_COUNT];
    static seq_t nxt[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i) cur[i] = nxt[i] = (seq_t)0;

    /* start state: previous row is all zeros (encoded as 4,4,4). */
    const int start_state = encode_state(4,4,4);
    cur[start_state] = (seq_t)1;

    for (idx_t row = 0; row < n; ++row)
    {
        for (int i = 0; i < STATE_COUNT; ++i) nxt[i] = (seq_t)0;

        for (int s = 0; s < STATE_COUNT; ++s)
        {
            seq_t ways = cur[s];
            if (ways == (seq_t)0) continue;

            for (int pat = 0; pat < PATTERN_COUNT; ++pat)
            {
                int ns = transition_table[s][pat];
                if (ns < 0) continue;
                nxt[ns] += ways;
            }
        }

        /* copy nxt into cur for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i) cur[i] = nxt[i];
    }

    /* Sum over all valid final states. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATE_COUNT; ++s)
        if (final_valid[s]) result += cur[s];

    return result;
}
