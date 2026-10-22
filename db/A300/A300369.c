/*
 * A300369  Number of nX5 0..1 arrays with every element equal to 2 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: This routine counts n×5 binary matrices (bits 0–1) in which
 * every cell has exactly 2 or 3 neighbours among the 6 directions:
 * horizontally left/right, vertically up/down, and anti‑diagonally up‑right
 * and down‑left.  The cell in the upper‑left corner is fixed to 0.  The
 * counting is performed by dynamic programming over rows; no closed formula
 * is known.  The DP state is the pair of the previous two rows.  The
 * sequence values are returned for all n ≥ 1 (a(1)=0).  Initialization of
 * a static table of valid row transitions is performed only once. */



#include <stdbool.h>

#define NO_ROW 32  /* sentinel index meaning 'row does not exist' */

static bool good[NO_ROW + 1][32][NO_ROW + 1];
static bool good_initialized = false;

/* Precompute whether a row `curr` satisfies its adjacency constraint,
 * given the presence of a neighboring row above (`prev`) and below
 * (`next`).  Indices NO_ROW denote an absent row. */
static void precompute_good(void)
{
    for (int pre = 0; pre <= NO_ROW; ++pre) {
        int prevMask = pre;            /* value is ignored when pre==NO_ROW */
        bool hasPrev = (pre != NO_ROW);
        for (int curr = 0; curr < 32; ++curr) {
            for (int nxt = 0; nxt <= NO_ROW; ++nxt) {
                int nextMask = nxt;
                bool hasNext = (nxt != NO_ROW);
                bool ok = true;

                for (int j = 0; j < 5; ++j) {
                    int b = (curr >> j) & 1;
                    int cnt = 0;

                    /* horizontal neighbors */
                    if (j > 0) {
                        int nb = (curr >> (j - 1)) & 1;
                        if (nb == b) ++cnt;
                    }
                    if (j < 4) {
                        int nb = (curr >> (j + 1)) & 1;
                        if (nb == b) ++cnt;
                    }

                    /* vertical neighbor above */
                    if (hasPrev) {
                        int nb = (prevMask >> j) & 1;
                        if (nb == b) ++cnt;
                    }

                    /* vertical neighbor below */
                    if (hasNext) {
                        int nb = (nextMask >> j) & 1;
                        if (nb == b) ++cnt;
                    }

                    /* anti‑diagonal up‑right */
                    if (hasPrev && j < 4) {
                        int nb = (prevMask >> (j + 1)) & 1;
                        if (nb == b) ++cnt;
                    }

                    /* anti‑diagonal down‑left */
                    if (hasNext && j > 0) {
                        int nb = (nextMask >> (j - 1)) & 1;
                        if (nb == b) ++cnt;
                    }

                    if (cnt != 2 && cnt != 3) {
                        ok = false;
                        break;
                    }
                }
                good[pre][curr][nxt] = ok;
            }
        }
    }
    good_initialized = true;
}

/* Count of n×5 arrays satisfying the adjacency rule.
 * idx_t is an unsigned 64‑bit integer type and seq_t is the integer
 * type used for the result.  The function is re‑entrant and works for
 * any n ≥ 1. */
seq_t a(idx_t n)
{
    if (!good_initialized)
        precompute_good();

    if (n < 2) {
        /* For a(1) the flag in the OEIS is 0. */
        return (seq_t)0;
    }

    static seq_t dp[32][32];
    static seq_t dp_next[32][32];

    /* reset dp arrays */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = (seq_t)0;

    /* Initialization: first row (mask1) must have upper‑left bit zero. */
    for (int mask1 = 0; mask1 < 32; ++mask1) {
        if ((mask1 & 1) != 0)            /* upper‑left must be 0 */
            continue;
        for (int mask2 = 0; mask2 < 32; ++mask2) {
            if (good[NO_ROW][mask1][mask2]) {
                dp[mask1][mask2] = (seq_t)1;
            }
        }
    }

    /* For rows 3 … n, propagate DP states */
    for (idx_t row = 3; row <= n; ++row) {
        /* reset dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = (seq_t)0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == (seq_t)0)
                    continue;
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (good[prev][curr][nxt]) {
                        dp_next[curr][nxt] += val;
                    }
                }
            }
        }

        /* copy dp_next into dp for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Final step: check constraints for the last row (no row below). */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            seq_t val = dp[prev][curr];
            if (val == (seq_t)0)
                continue;
            if (good[prev][curr][NO_ROW]) {
                result += val;
            }
        }
    }

    return result;
}
