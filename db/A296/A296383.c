/*
 * A296383  Number of nX5 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A296383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A296383: Number of n×5 0/1 arrays with each 1 horizontally, vertically
    or antidiagonally adjacent to 1 or 2 neighboring 1s.
    The algorithm enumerates configurations column by column, tracking
    for each column whether a 1 exists and its current degree
    (0–2).  The degree counts are updated with horizontal, vertical
    and the correct antidiagonal (NE–SW) adjacency.  States are
    stored compactly in 10 bits (2 bits per column).  The transition
    tables are built once and memoised.  The function is re‑entrant
    and works for any seq_t/idx_t type. */

#include <stdlib.h>
#include <stdbool.h>

#define COLS 5
#define STATE_BITS (2 * COLS)                  /* 10 bits for 5 columns */
#define STATE_COUNT (1 << STATE_BITS)          /* 1024 states */

/* ------------------------------------------------------------------ */
/*  Early initialisation of transition tables.                         */
/* ------------------------------------------------------------------ */
static bool init_done = false;
static unsigned short trans_ns[STATE_COUNT][32];
static unsigned char  trans_cnt[STATE_COUNT][32];
static unsigned char  trans_len[STATE_COUNT];

static void initialise(void) {
    if (init_done) return;
    init_done = true;

    /* Iterate over all possible previous‑row states. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int prev_bit[COLS];
        int prev_deg[COLS];                  /* -1 = no 1, else 0..2 */

        /* Decode the state. */
        for (int c = 0; c < COLS; ++c) {
            int val = (s >> (2 * c)) & 3;
            prev_bit[c] = (val != 0);
            prev_deg[c] = (val == 0) ? -1 : (val - 1);
        }

        int ns_arr[32];
        int ns_len = 0;

        /* Try every possible current‑row pattern (5 bits). */
        for (int p = 0; p < (1 << COLS); ++p) {
            int curr_bit[COLS];
            int curr_deg[COLS] = {0};
            int prev_deg_new[COLS];

            for (int c = 0; c < COLS; ++c) {
                curr_bit[c] = (p >> c) & 1;
                prev_deg_new[c] = prev_deg[c];
            }

            /* Horizontal edges within the current row. */
            for (int c = 0; c < COLS - 1; ++c)
                if (curr_bit[c] && curr_bit[c + 1]) {
                    curr_deg[c]++; curr_deg[c + 1]++; }
            /* Vertical edges to the previous row. */
            for (int c = 0; c < COLS; ++c)
                if (curr_bit[c] && prev_bit[c]) {
                    curr_deg[c]++; prev_deg_new[c]++; }
            /* Antidiagonal edges (NE‑SW) from current to previous. */
            for (int c = 0; c < COLS - 1; ++c)
                if (curr_bit[c] && prev_bit[c + 1]) {
                    curr_deg[c]++; prev_deg_new[c + 1]++; }

            /* Check the previous row cells (now finalized). */
            bool ok = true;
            for (int c = 0; c < COLS; ++c) {
                if (!prev_bit[c]) continue;
                int d = prev_deg_new[c];
                if (d < 1 || d > 2) { ok = false; break; }
            }
            if (!ok) continue;

            /* Current row cells must not exceed degree 2 yet. */
            for (int c = 0; c < COLS; ++c)
                if (curr_bit[c] && curr_deg[c] > 2) { ok = false; break; }
            if (!ok) continue;

            /* Encode the resulting state of the current row. */
            int ns = 0;
            for (int c = 0; c < COLS; ++c) {
                int val = (curr_bit[c] ? curr_deg[c] + 1 : 0);
                ns |= (val << (2 * c));
            }
            ns_arr[ns_len++] = ns;
        }

        /* Collapse duplicates and count multiplicities. */
        int unique_ns[32];
        int unique_cnt[32];
        int unique_len = 0;
        for (int i = 0; i < ns_len; ++i) {
            int ns = ns_arr[i];
            int found = -1;
            for (int j = 0; j < unique_len; ++j)
                if (unique_ns[j] == ns) { found = j; break; }
            if (found == -1) {
                unique_ns[unique_len] = ns;
                unique_cnt[unique_len] = 1;
                unique_len++;
            } else {
                unique_cnt[found]++; }
        }
        trans_len[s] = (unsigned char)unique_len;
        for (int j = 0; j < unique_len; ++j) {
            trans_ns[s][j]  = (unsigned short)unique_ns[j];
            trans_cnt[s][j] = (unsigned char)unique_cnt[j];
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Main DP function.                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    initialise();

    /* Allocate DP arrays. */
    seq_t *dp_cur = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
    seq_t *dp_nxt = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
    if (!dp_cur || !dp_nxt) return (seq_t)0;    /* safety */

    dp_cur[0] = (seq_t)1;   /* start with an empty previous row */

    for (idx_t row = 1; row <= n; ++row) {
        /* clear next array */
        for (int i = 0; i < STATE_COUNT; ++i) dp_nxt[i] = (seq_t)0;

        /* transition from all states of the previous row */
        for (int s = 0; s < STATE_COUNT; ++s) {
            if (dp_cur[s] == (seq_t)0) continue;
            for (int k = 0; k < trans_len[s]; ++k) {
                int ns   = trans_ns[s][k];
                int cnt  = trans_cnt[s][k];
                /* each of the cnt patterns contributes independently */
                dp_nxt[ns] += dp_cur[s] * (seq_t)cnt;
            }
        }

        /* swap arrays */
        seq_t *tmp = dp_cur; dp_cur = dp_nxt; dp_nxt = tmp;
    }

    /* Sum all final states that have no 1 with degree 0. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        if (dp_cur[s] == (seq_t)0) continue;
        bool ok = true;
        for (int c = 0; c < COLS; ++c) {
            int val = (s >> (2 * c)) & 3;
            if (val == 1) { ok = false; break; }   /* present 1 with deg 0 */
        }
        if (ok) result += dp_cur[s];
    }

    free(dp_cur);
    free(dp_nxt);
    return result;
}
