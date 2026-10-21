/*
 * A296987  Number of nX5 0..1 arrays with each 1 adjacent to 3, 4 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A296987
 *  Number of n×5 0..1 arrays in which each 1 has between 3 and 5
 *  king-move neighboring 1s.
 *
 *  The implementation uses dynamic programming over the last two rows.
 *  States are encoded as (prevprev<<5 | prev); transitions are allowed
 *  only when the middle row satisfies the neighbor‑count restriction.
 *  Tables of validity for first/middle/last rows are pre‑computed once.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Check whether all 1s in 'cur' have 3–5 1‑neighbors using the rows
 *  above ('prevprev') and below ('next').  When there is no next row
 *  ('has_next' == false) the below row is ignored.
 */
static bool valid_row(unsigned prevprev, unsigned cur, unsigned next,
                      bool has_next)
{
    for (unsigned j = 0; j < 5; ++j) {
        if ((cur >> j) & 1U) {
            unsigned cnt = 0;
            /* left / right in the same row */
            if (j > 0U && ((cur >> (j - 1)) & 1U)) ++cnt;
            if (j < 4U && ((cur >> (j + 1)) & 1U)) ++cnt;
            /* up */
            if ((prevprev >> j) & 1U) ++cnt;
            /* up‑left / up‑right */
            if (j > 0U      && ((prevprev >> (j - 1)) & 1U)) ++cnt;
            if (j < 4U      && ((prevprev >> (j + 1)) & 1U)) ++cnt;
            /* down (if present) */
            if (has_next && ((next >> j) & 1U)) ++cnt;
            /* down‑left / down‑right */
            if (has_next && j > 0U      && ((next >> (j - 1)) & 1U)) ++cnt;
            if (has_next && j < 4U      && ((next >> (j + 1)) & 1U)) ++cnt;
            /* require 3 ≤ cnt ≤ 5 */
            if (cnt < 3U || cnt > 5U) return false;
        }
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Transition tables (built once) */
static bool tables_initialized = false;
static bool first_valid[32][32];          /* first row against second */
static bool middle_valid[32][32][32];     /* middle row with neighbors */
static bool last_valid[32][32];           /* last row against previous */

static void build_tables(void)
{
    for (unsigned a = 0; a < 32U; ++a) {
        for (unsigned b = 0; b < 32U; ++b) {
            first_valid[a][b] = valid_row(0U, a, b, true);
            for (unsigned c = 0; c < 32U; ++c)
                middle_valid[a][b][c] = valid_row(a, b, c, true);
            last_valid[a][b] = valid_row(a, b, 0U, false);
        }
    }
    tables_initialized = true;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function */
seq_t a(idx_t n)
{
    if (!tables_initialized)
        build_tables();

    /* n = 0 never requested; the first term is a(1) = 1. */
    if (n == 1U) {
        seq_t count = 0;
        for (unsigned a = 0; a < 32U; ++a)
            if (last_valid[0][a])  /* row with no above and no below */
                count += 1;
        return count;
    }

    const size_t STATES = 1024U;          /* 10‑bit state: (prevprev<<5|prev) */
    seq_t *dp = (seq_t*)calloc(STATES, sizeof(seq_t));
    if (!dp) return (seq_t)0;            /* unlikely allocation failure */

    /* Initial states after processing the first two rows */
    for (unsigned a = 0; a < 32U; ++a) {
        for (unsigned b = 0; b < 32U; ++b) {
            if (first_valid[a][b])
                dp[(a << 5U) | b] = 1;
        }
    }

    /* Add rows 3 … n (zero‑based) */
    for (idx_t step = 2U; step < n; ++step) {
        seq_t *next = (seq_t*)calloc(STATES, sizeof(seq_t));
        if (!next) { free(dp); return (seq_t)0; }

        for (size_t s = 0U; s < STATES; ++s) {
            seq_t cur_val = dp[s];
            if (!cur_val) continue;
            unsigned a = (unsigned)(s >> 5U);   /* prevprev */
            unsigned b = (unsigned)(s & 31U);   /* prev   */

            for (unsigned c = 0; c < 32U; ++c) {
                if (middle_valid[a][b][c]) {
                    next[(b << 5U) | c] += cur_val;
                }
            }
        }
        free(dp);
        dp = next;
    }

    /* Final validation of the last row */
    seq_t result = 0;
    for (size_t s = 0U; s < STATES; ++s) {
        seq_t cur_val = dp[s];
        if (!cur_val) continue;
        unsigned a = (unsigned)(s >> 5U);   /* prevprev */
        unsigned b = (unsigned)(s & 31U);   /* prev   */
        if (last_valid[a][b])
            result += cur_val;
    }

    free(dp);
    return result;
}
