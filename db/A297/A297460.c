/*
 * A297460  Number of 4Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence‑based DP over column states for A297460 (4×n 0–1 arrays
   with every 1 having 0 or 2 neighbours horizontally, diagonally or
   anti‑diagonally).  The sequence is computed by dynamic programming
   over patterns of one column and the number of left‑neighbour edges
   each 1 has coming from the previous column. */

#include <stdbool.h>

/* Constants.  All are compile‑time integral constants. */
enum {
    ROWS            = 4,
    PATTERN_BITS    = 4,
    PATTERN_COUNT   = 1 << PATTERN_BITS,    /* 16 */
    COUNT_BASE      = 4,                    /* 0..3 left‑neighbour counts */
    STATE_COUNT     = PATTERN_COUNT * COUNT_BASE *
                      COUNT_BASE * COUNT_BASE *
                      COUNT_BASE,          /* 16 * 4^4 = 4096 */
};

/* State of a column: 4‑bit pattern of 1‑cells + 4 base‑4 digits that
   encode, for each row, the number of left‑neighbour edges this cell
   has from the preceding column.  The least significant bits store
   the pattern. */
static int trans[STATE_COUNT][PATTERN_COUNT];   /* next state id or -1 */
static bool final_valid[STATE_COUNT];           /* true if state can be final */

/* Decode a state into its pattern and the base‑4 counts for each row. */
static void decode_state(int state, int *pattern, int cnt[ROWS]) {
    *pattern = state & (PATTERN_COUNT - 1);          /* lower 4 bits */
    int idx   = state >> PATTERN_BITS;               /* remaining digits */
    for (int r = 0; r < ROWS; ++r) {
        cnt[r] = idx % COUNT_BASE;
        idx   /= COUNT_BASE;
    }
}

/* One‑time construction of the transition table.  This is performed
   lazily when a() is first called. */
static void init_transitions(void) {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* Fill in trans[s][q] and final_valid[s]. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int p, cnt[ROWS];
        decode_state(s, &p, cnt);

        /* Evaluate all possible next‑column patterns q. */
        for (int q = 0; q < PATTERN_COUNT; ++q) {
            int ok = 1;
            /* Check that each 1 in the current column can have 0 or 2
               neighbours when combined with the right‑neighbour counts
               coming from the pattern q. */
            for (int r = 0; r < ROWS && ok; ++r) {
                if ((p >> r) & 1) {
                    int right = (q >> r) & 1;
                    if (r > 0) right += (q >> (r - 1)) & 1;
                    if (r < ROWS - 1) right += (q >> (r + 1)) & 1;
                    int total = cnt[r] + right;
                    if (total != 0 && total != 2) ok = 0;
                }
            }

            if (!ok) {
                trans[s][q] = -1;
                continue;
            }

            /* Compute the left‑neighbour count for the next column.
               For each row of the next column the left neighbours are
               the cells in the current column at the same row and the two
               diagonally adjacent rows. */
            int newcnt = 0;
            int mul    = 1;
            for (int r = 0; r < ROWS; ++r) {
                int contrib = (p >> r) & 1;
                if (r > 0)     contrib += (p >> (r - 1)) & 1;
                if (r < ROWS - 1) contrib += (p >> (r + 1)) & 1;
                newcnt += contrib * mul;
                mul   *= COUNT_BASE;
            }

            trans[s][q] = (newcnt << PATTERN_BITS) | q;
        }

        /* Check whether this state may be valid as the last column. */
        bool ok = 1;
        for (int r = 0; r < ROWS && ok; ++r) {
            if ((p >> r) & 1) {
                int total = cnt[r];                 /* no right neighbours */
                if (total != 0 && total != 2) ok = 0;
            }
        }
        final_valid[s] = ok;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    init_transitions();

    static seq_t dp_curr[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i) dp_curr[i] = 0;

    /* Initialise with the first column: no left neighbours. */
    for (int p = 0; p < PATTERN_COUNT; ++p) dp_curr[p] = (seq_t)1;

    /* Process remaining columns. */
    for (idx_t col = 1; col < n; ++col) {
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t ways = dp_curr[s];
            if (ways == 0) continue;
            for (int q = 0; q < PATTERN_COUNT; ++q) {
                int ns = trans[s][q];
                if (ns >= 0) dp_next[ns] += ways;
            }
        }
        /* Move to next column. */
        for (int i = 0; i < STATE_COUNT; ++i) dp_curr[i] = dp_next[i];
    }

    /* Sum over valid final states. */
    seq_t ans = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        if (dp_curr[s] == 0) continue;
        if (final_valid[s]) ans += dp_curr[s];
    }
    return ans;
}
