/*
 * A298442  Number of n X 6 0..1 arrays with every element equal to 0, 3, 4, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×6 binary arrays with the king‑ move adjacency
 *            rule * (every cell has 0,3,4,5, or 6 equal neighbours, upper
 *            left cell 0). * * The implementation uses dynamic programming
 *            over row pairs. * States: pair of consecutive rows (prev, curr),
 *            encoded as 12‑bit integers. * For each state...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static const bool ALLOWED[9] = { true, false, false, true, true, true, true, false, false };

/* Pre‑computed transition tables:
 * NEXT_CNT[state]  = number of next rows allowed for that state
 * NEXT_ROW[state][k] = the k‑th allowed next row
 */
static uint8_t NEXT_CNT[4096];
static uint8_t NEXT_ROW[4096][64];
static bool TRANS_INIT = false;

/* Checks if a row (curr) satisfies the adjacency rule when it has
 * optional upper row (prev, with flag hasPrev) and optional lower row (next, with flag hasNext).
 */
static inline bool row_satisfied(int curr, int prev, bool hasPrev, int next, bool hasNext)
{
    for (int col = 0; col < 6; ++col) {
        int val = (curr >> col) & 1;
        int cnt = 0;

        /* neighbors in the same row */
        if (col > 0   && ((curr >> (col-1)) & 1) == val) ++cnt;
        if (col < 5   && ((curr >> (col+1)) & 1) == val) ++cnt;

        /* upper neighbours */
        if (hasPrev) {
            if (col > 0   && ((prev >> (col-1)) & 1) == val) ++cnt;
            if (          ((prev >> col)     & 1) == val) ++cnt;
            if (col < 5   && ((prev >> (col+1)) & 1) == val) ++cnt;
        }

        /* lower neighbours */
        if (hasNext) {
            if (col > 0   && ((next >> (col-1)) & 1) == val) ++cnt;
            if (          ((next >> col)     & 1) == val) ++cnt;
            if (col < 5   && ((next >> (col+1)) & 1) == val) ++cnt;
        }

        if (!ALLOWED[cnt]) return false;
    }
    return true;
}

/* Convenience wrappers for corner cases */
static inline bool top_row_valid(int row0, int row1)
{
    return row_satisfied(row0, 0, false, row1, true);
}

static inline bool bottom_row_valid(int row0, int row1)
{
    return row_satisfied(row1, row0, true, 0, false);
}

static inline bool middle_row_valid(int prev, int curr, int next)
{
    return row_satisfied(curr, prev, true, next, true);
}

/* Builds the transition tables once */
static void init_transitions(void)
{
    if (TRANS_INIT) return;
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            int id = (prev << 6) | curr;
            int cnt = 0;
            for (int next = 0; next < 64; ++next) {
                if (middle_row_valid(prev, curr, next))
                    NEXT_ROW[id][cnt++] = (uint8_t)next;
            }
            NEXT_CNT[id] = (uint8_t)cnt;
        }
    }
    TRANS_INIT = true;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    init_transitions();

    /* DP arrays over state (prev<<6 | curr) */
    seq_t *dp     = (seq_t *)calloc(4096, sizeof(seq_t));
    seq_t *nextdp = (seq_t *)calloc(4096, sizeof(seq_t));
    if (!dp || !nextdp) return (seq_t)0;  /* memory fault protection */

    /* initialise for the first two rows */
    for (int row0 = 0; row0 < 64; ++row0) {
        if (row0 & 1) continue;   /* upper left element must be 0 */
        for (int row1 = 0; row1 < 64; ++row1)
            if (top_row_valid(row0, row1))
                dp[(row0 << 6) | row1] = (seq_t)1;
    }

    if (n == 2) {
        seq_t total = (seq_t)0;
        for (int s = 0; s < 4096; ++s) {
            if (dp[s] == (seq_t)0) continue;
            int prev = s >> 6;
            int curr = s & 63;
            if (bottom_row_valid(prev, curr))
                total += dp[s];
        }
        free(dp); free(nextdp);
        return total;
    }

    /* n >= 3 : iterate remaining rows */
    for (idx_t step = 0; step < n-2; ++step) {
        /* clear nextdp */
        for (idx_t i = 0; i < 4096; ++i) nextdp[i] = (seq_t)0;

        for (idx_t s = 0; s < 4096; ++s) {
            seq_t ways = dp[s];
            if (ways == (seq_t)0) continue;

            int prev = s >> 6;
            int curr = s & 63;
            int cnt  = NEXT_CNT[s];

            for (int k = 0; k < cnt; ++k) {
                int next   = NEXT_ROW[s][k];
                idx_t nst  = ((idx_t)curr << 6) | (idx_t)next;
                nextdp[nst] += ways;
            }
        }
        /* swap dp and nextdp */
        seq_t *tmp  = dp; dp = nextdp; nextdp = tmp;
    }

    /* final row (bottom) validation */
    seq_t total = (seq_t)0;
    for (int s = 0; s < 4096; ++s) {
        if (dp[s] == (seq_t)0) continue;
        int prev = s >> 6;
        int curr = s & 63;
        if (bottom_row_valid(prev, curr))
            total += dp[s];
    }

    free(dp); free(nextdp);
    return total;
}
