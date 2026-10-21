/*
 * A297753  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 3 or 4 neighboring 1s.
 * https://oeis.org/A297753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays in which each 1 has 1, 3 or 4
 *            neighboring 1s among the six horizontal/diagonal directions
 *            (horizontal, diagonal, antidiagonal). The value is computed by a
 *            transfer‑matrix dynamic program
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* A column mask is a 5‑bit number (bits 0..4). */
    enum { COLS   = 32,                /* 2^5 */
           STATES = COLS * COLS };      /* (prev, curr) pairs */

    /* Precomputation of validity and transition lists. */
    static bool  valid[STATES * COLS]   = {0};     /* valid[prev*32*32 + curr*32 + next] */
    static unsigned char nexts[STATES][COLS];      /* next columns allowed from each state */
    static unsigned char next_cnt[STATES] = {0};
    static bool prepared = false;

    if (!prepared) {
        for (unsigned int p = 0; p < COLS; ++p) {
            for (unsigned int c = 0; c < COLS; ++c) {
                unsigned int state = (p << 5) | c;
                unsigned int cnt   = 0;
                for (unsigned int ncol = 0; ncol < COLS; ++ncol) {
                    bool ok = true;
                    for (int r = 0; r < 5 && ok; ++r) {
                        if (!(c & (1U << r))) continue;   /* cell is 0 -> no check */

                        unsigned int neigh = 0;
                        /* neighbors from the left column */
                        if (p & (1U << r))          neigh++;   /* horizontal left */
                        if (r > 0 && (p & (1U << (r - 1)))) neigh++; /* left diagonal up */
                        if (r < 4 && (p & (1U << (r + 1)))) neigh++; /* left diagonal down */
                        /* neighbors from the right column */
                        if (ncol & (1U << r))          neigh++;   /* horizontal right */
                        if (r > 0 && (ncol & (1U << (r - 1)))) neigh++; /* right diagonal up */
                        if (r < 4 && (ncol & (1U << (r + 1)))) neigh++; /* right diagonal down */
                        /* vertical neighbours are NOT counted in this definition */
                        if (!(neigh == 1 || neigh == 3 || neigh == 4))
                            ok = false;
                    }
                    if (ok) {
                        valid[state * COLS + ncol] = true;
                        nexts[state][cnt] = (unsigned char)ncol;
                        ++cnt;
                    }
                }
                next_cnt[state] = (unsigned char)cnt;
            }
        }
        prepared = true;
    }

    /* Dynamic programming arrays. */
    seq_t *dp      = (seq_t *)calloc(STATES, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(STATES, sizeof(seq_t));
    if (!dp || !dp_next) return (seq_t)0;   /* allocation failure */

    /* Initialise with left boundary = 0 (prev column all zeros). */
    for (unsigned int c = 0; c < COLS; ++c) {
        unsigned int idx = (0 << 5) | c;   /* prev=0 */
        dp[idx] = 1;
    }

    /* Progress through columns 2..n. */
    for (idx_t step = 1; step < n; ++step) {
        for (unsigned int state = 0; state < STATES; ++state) {
            seq_t val = dp[state];
            if (val == (seq_t)0) continue;
            unsigned int curr = state & 0x1F;
            unsigned char tc = next_cnt[state];
            for (unsigned int k = 0; k < tc; ++k) {
                unsigned int nextcol   = nexts[state][k];
                unsigned int next_state = (curr << 5) | nextcol;
                dp_next[next_state] += val;
            }
        }
        /* Swap buffers and clear dp_next. */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
        for (unsigned int i = 0; i < STATES; ++i) dp_next[i] = 0;
    }

    /* Final column must satisfy rule with right boundary = 0. */
    seq_t result = 0;
    for (unsigned int state = 0; state < STATES; ++state) {
        if (valid[state * COLS + 0])   /* right neighbour = 0 */
            result += dp[state];
    }

    free(dp);
    free(dp_next);
    return result;
}
