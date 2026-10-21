/*
 * A250569  Number of (n+2) X (3+2) 0..1 arrays with nondecreasing maximum of
 *          every three consecutive values in every row and column.
 * https://oeis.org/A250569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Transfer‑matrix DP for OEIS sequence A250569
 *  a(n) = number of (n+2)×5 binary arrays such that
 *  for every row and for every column the maximum of each
 *  consecutive triple of entries is a nondecreasing sequence.
 *  The algorithm counts matrices by adding rows one by one.
 *  Each column is modelled by a 4‑state automaton that remembers
 *  whether a 1 has appeared yet and the length (0,1,2) of the
 *  most recent run of zeros after the first 1.  The transition
 *  from a state by a bit 0 or 1 is predefined.  A row pattern
 *  (5 bits) is allowed iff it does not contain the forbidden
 *  pattern 1 followed by three zeros; there are 21 such
 *  patterns out of the 32 possible ones.  The DP state is the
 *  tuple of the five column states, encoded as a single base‑4
 *  number (4⁵ = 1024 states).  For each step we update the
 *  DP array using a pre‑computed transition table.
 *  Complexity: O(n·STATES·PATTERNS) ≈ O(n) with small constants.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* Constants */
#define N_COL    5
#define STATES  (1024)          /* 4⁵ */
#define PATTERNS (32)          /* 2⁵ */

/*  Transition table: trans[state][pattern] = next_state or -1 if forbidden */
static short trans_table[STATES][PATTERNS];

/*  Row validity table: row_valid[p] = 1 if pattern p is allowed in a row */
static unsigned char row_valid[PATTERNS];

/*  One‑time initialization flag and routine */
static void init_tables(void) {
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /* Precompute which 5‑bit patterns are valid for a row
     * (no 1 followed by three consecutive zeros). */
    for (int p = 0; p < PATTERNS; ++p) {
        int valid = 1;
        for (int i = 0; i <= 1; ++i) {            /* check positions 0..3 and 1..4 */
            if (((p >> i) & 1) &&
                !((p >> (i + 1)) & 1) &&
                !((p >> (i + 2)) & 1) &&
                !((p >> (i + 3)) & 1))
            {
                valid = 0;
                break;
            }
        }
        row_valid[p] = (unsigned char)valid;
    }

    /* Automaton transition for a single column:
     * 0 – no 1 seen yet
     * 1 – last run of zeros length 0 (last bit 1)
     * 2 – last run of zeros length 1
     * 3 – last run of zeros length 2
     *
     * ny = nxt[old_state][bit] where bit is 0 or 1. */
    int nxt[4][2] = {
        {0, 1},    /* from state 0: 0→0, 1→1 */
        {2, 1},    /* from state 1: 0→2, 1→1 */
        {3, 1},    /* from state 2: 0→3, 1→1 */
        {-1, 1}    /* from state 3: 0→−1 (invalid), 1→1 */
    };

    /* Build the full transition table from column states to new column states. */
    for (int s = 0; s < STATES; ++s) {
        int tint = s;
        int col_state[N_COL];
        for (int c = 0; c < N_COL; ++c) {
            col_state[c] = tint & 3;   /* base‑4 digit */
            tint >>= 2;
        }
        for (int p = 0; p < PATTERNS; ++p) {
            if (!row_valid[p]) {
                trans_table[s][p] = -1;
                continue;
            }
            int new_states[N_COL];
            int ok = 1;
            for (int c = 0; c < N_COL; ++c) {
                int bit = (p >> c) & 1;
                int ns = nxt[col_state[c]][bit];
                if (ns < 0) { ok = 0; break; }
                new_states[c] = ns;
            }
            if (!ok) {
                trans_table[s][p] = -1;
            } else {
                int ns = new_states[0] |
                         (new_states[1] << 2) |
                         (new_states[2] << 4) |
                         (new_states[3] << 6) |
                         (new_states[4] << 8);
                trans_table[s][p] = (short)ns;
            }
        }
    }
}

/*  Main enumeration function */
seq_t a(idx_t n) {
    init_tables();

    /* Each matrix has n+2 rows. */
    size_t rows = (size_t)n + 2;

    /* Two DP arrays; we will swap pointers through each iteration. */
    static seq_t cur[STATES];
    static seq_t nxt[STATES];
    seq_t *curp = cur;
    seq_t *nxtp = nxt;

    /* Initial state: all columns are in state 0 (no 1 seen). */
    memset(curp, 0, sizeof(cur));
    curp[0] = 1;

    for (size_t r = 0; r < rows; ++r) {
        memset(nxtp, 0, sizeof(nxt));
        for (int s = 0; s < STATES; ++s) {
            seq_t val = curp[s];
            if (val == 0) continue;
            for (int p = 0; p < PATTERNS; ++p) {
                int ns = trans_table[s][p];
                if (ns >= 0) {
                    nxtp[ns] += val;
                }
            }
        }
        /* swap the DP arrays for next iteration */
        seq_t *tmp = curp;
        curp = nxtp;
        nxtp = tmp;
    }

    /* Sum over all states to obtain the answer. */
    seq_t ans = 0;
    for (int s = 0; s < STATES; ++s) {
        ans += curp[s];
    }
    return ans;
}
