/*
 * A296722  Number of nX5 0..1 arrays with each 1 adjacent to 0, 2 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays where each 1 has 0, 2 or 4 *
 *            king‑move neighboring 1s
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a transfer‑matrix dynamic program over
 * the 5 columns.  Each state records for every column whether a 1 is
 * present and the partial number of neighboring 1s that have been
 * seen so far (from the previous row and the current row).  The
 * partial counts are 0…5; a missing 1 is encoded by the value 6.
 * After processing a row the counts for the row above are finalized
 * and checked against the set {0,2,4}.  After all rows the counts
 * of the last row are also checked.  A table of all 7^5 = 16807
 * possible states and their transitions for 32 possible row patterns
 * is pre‑computed once.  The DP runs in O(n·m) where m is the number
 * of active states (typically a few hundred). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define STATE_BASE 7
#define STATE_COLS 5
#define PATTERN_COUNT 32
#define STATE_COUNT 16807   /* 7^5 */

static int16_t next_state[STATE_COUNT][PATTERN_COUNT];
static uint8_t state_digit[STATE_COUNT][STATE_COLS];
static int state_mask[STATE_COUNT];
static uint8_t h_adj[PATTERN_COUNT][STATE_COLS];
static uint8_t delta_pat[PATTERN_COUNT][STATE_COLS];
static int neighbor_mask[STATE_COLS];

static void init(void)
{
    static bool done = false;
    if (done)
        return;
    done = true;

    /* neighbor mask for each column (silver wire): bits of adjacent columns */
    for (int j = 0; j < STATE_COLS; ++j) {
        int m = 0;
        for (int k = j - 1; k <= j + 1; ++k) {
            if (k >= 0 && k < STATE_COLS)
                m |= 1 << k;
        }
        neighbor_mask[j] = m;
    }

    /* Pre‑compute horizontal adjacency for each pattern */
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        for (int j = 0; j < STATE_COLS; ++j) {
            int cnt = 0;
            if (j > 0 && (p >> (j - 1)) & 1)
                ++cnt;
            if (j < STATE_COLS - 1 && (p >> (j + 1)) & 1)
                ++cnt;
            h_adj[p][j] = (uint8_t)cnt;
            /* vertical adjacency from pattern to previous row cell j  */
            delta_pat[p][j] = (uint8_t)__builtin_popcount(p & neighbor_mask[j]);
        }
    }

    /* Generate all states and pre‑compute transitions */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int t = s;
        int mask = 0;
        for (int j = 0; j < STATE_COLS; ++j) {
            uint8_t d = t % STATE_BASE;
            t /= STATE_BASE;
            state_digit[s][j] = d;                     /* 0..6 ; 6 means empty */
            if (d != 6)
                mask |= 1 << j;
        }
        state_mask[s] = mask;
        for (int p = 0; p < PATTERN_COUNT; ++p) {
            bool ok = true;
            /* validate previous row cells */
            for (int k = 0; k < STATE_COLS; ++k) {
                if (state_digit[s][k] == 6)
                    continue;
                int prev_partial = state_digit[s][k];
                int delta = delta_pat[p][k];
                int total = prev_partial + delta;
                if (total != 0 && total != 2 && total != 4) {
                    ok = false;
                    break;
                }
            }
            if (!ok) {
                next_state[s][p] = -1;
                continue;
            }
            /* compute new state digits */
            uint8_t new_digits[STATE_COLS];
            for (int j = 0; j < STATE_COLS; ++j) {
                if ((p >> j) & 1) {
                    int horiz = h_adj[p][j];
                    int vert = __builtin_popcount(state_mask[s] & neighbor_mask[j]);
                    new_digits[j] = (uint8_t)(horiz + vert);  /* 0..5 */
                } else {
                    new_digits[j] = 6;  /* empty */
                }
            }
            int ns = 0;
            for (int j = STATE_COLS - 1; j >= 0; --j)
                ns = ns * STATE_BASE + new_digits[j];
            next_state[s][p] = (int16_t)ns;
        }
    }
}

static const uint8_t allowed[8] = {1,0,1,0,1,0,0,0};
/* allowed[v] == 1 for v==0,2,4 */

seq_t a(idx_t n)
{
    init();

    if (n == 0)
        return 0; /* undefined offset, return 0 */

    /* DP arrays */
    static seq_t *dp_cur = NULL;
    static seq_t *dp_next = NULL;
    static int *active = NULL;
    static int *new_active = NULL;
    static int *last_seen = NULL;
    static int epoch = 1;

    if (!dp_cur) {
        dp_cur = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
        dp_next = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
        active = (int *)malloc(STATE_COUNT * sizeof(int));
        new_active = (int *)malloc(STATE_COUNT * sizeof(int));
        last_seen = (int *)malloc(STATE_COUNT * sizeof(int));
        for (int i = 0; i < STATE_COUNT; ++i)
            last_seen[i] = 0;
    }

    /* initialise */
    for (int i = 0; i < STATE_COUNT; ++i)
        dp_cur[i] = 0;
    int start_state = STATE_COUNT - 1; /* all digits = 6 */
    dp_cur[start_state] = 1;
    int active_size = 1;
    active[0] = start_state;

    /* DP over rows */
    for (idx_t row = 0; row < n; ++row) {
        int new_size = 0;
        int cur_epoch = epoch++;
        for (int idx = 0; idx < active_size; ++idx) {
            int s = active[idx];
            seq_t val = dp_cur[s];
            for (int p = 0; p < PATTERN_COUNT; ++p) {
                int ns = next_state[s][p];
                if (ns == -1)
                    continue;
                dp_next[ns] += val;
                if (last_seen[ns] != cur_epoch) {
                    last_seen[ns] = cur_epoch;
                    new_active[new_size++] = ns;
                }
            }
            dp_cur[s] = 0; /* reset for reuse */
        }
        /* swap arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        /* move active list */
        active_size = new_size;
        for (int i = 0; i < active_size; ++i)
            active[i] = new_active[i];
    }

    /* final evaluation */
    seq_t result = 0;
    for (int i = 0; i < active_size; ++i) {
        int s = active[i];
        bool ok = true;
        for (int j = 0; j < STATE_COLS; ++j) {
            if (state_digit[s][j] == 6)
                continue;
            if (!allowed[state_digit[s][j]]) {
                ok = false;
                break;
            }
        }
        if (ok)
            result += dp_cur[s];
    }
    return result;
}
