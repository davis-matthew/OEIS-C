/*
 * A296640  Number of nX5 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 1, 2 or 4 neighboring 1s.
 * https://oeis.org/A296640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function counts the number of n×5 binary arrays in which
 *            each 1 has 1, 2 or 4 neighbours among the horizontal, vertical,
 *            or antidiagonal neighbours (up‑right and down‑left)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses dynamic programming over triples of rows.
   For every pair (prev, cur) it stores the number of ways to reach
   that state after processing so far.  When a new row `next` is added,
   we check that each 1 in `cur` has a correct number of neighbours
   using `prev`, `cur`, and `next`.  After all n rows are processed,
   we finally check that the last row is valid with a fictitious
   zero row below.  The state space is only 32×32, so the method is
   O(n) with a very small constant.  All arithmetic uses the type
   seq_t supplied by the build. */

#include <stdint.h>
#include <stdbool.h>

/* Transition tables – built once on first call. */
static bool                prv_valid[32][32][32];
static uint8_t             prv_transitions[32][32][32];
static uint8_t             prv_trans_count[32][32];
static bool                prv_initialized = false;

/* Pre‑compute valid triples (prev,cur,next) and
   store the list of admissible next rows for each (prev,cur). */
static void prv_precompute(void)
{
    if (prv_initialized) return;

    for (int prev = 0; prev < 32; ++prev) {
        for (int cur  = 0; cur  < 32; ++cur) {
            prv_trans_count[prev][cur] = 0;
            for (int next = 0; next < 32; ++next) {
                bool ok = true;

                /* check each 1 in the current row */
                for (int c = 0; c < 5; ++c) {
                    if (!(cur & (1 << c))) continue;          /* 0 → skip */

                    int neigh = 0;

                    /* horizontal neighbours */
                    if (c > 0     && (cur & (1 << (c-1)))) ++neigh;
                    if (c < 4     && (cur & (1 << (c+1)))) ++neigh;

                    /* vertical neighbours */
                    if (prev & (1 << c)) ++neigh;              /* up */
                    if (next & (1 << c)) ++neigh;              /* down */

                    /* antidiagonal neighbours (up‑right, down‑left) */
                    if (c < 4 && (prev & (1 << (c+1)))) ++neigh;   /* up‑right */
                    if (c > 0 && (next & (1 << (c-1)))) ++neigh;   /* down‑left */

                    if (!((neigh == 1) || (neigh == 2) || (neigh == 4))) {
                        ok = false;
                        break;
                    }
                }

                prv_valid[prev][cur][next] = ok;
                if (ok) {
                    prv_transitions[prev][cur][prv_trans_count[prev][cur]++] =
                        (uint8_t)next;
                }
            }
        }
    }
    prv_initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n >= 1 for the sequence.  For n==0 return 0 (undefined). */
    if (n == 0) return (seq_t)0;

    prv_precompute();

    /* DP arrays – allocated locally to avoid stale data from previous calls. */
    seq_t dp[32][32];
    seq_t ndp[32][32];

    /* initialise dp to zero */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = (seq_t)0;

    /* Initial state: no prior row (all zeros), first row is `cur`. */
    for (int cur = 0; cur < 32; ++cur) {
        dp[0][cur] = (seq_t)1;
    }

    /* Process rows 2 … n.  After each step, `dp[prev][cur]` holds the
       number of ways to obtain the last two rows as (prev,cur). */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear ndp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = (seq_t)0;

        /* transition from (prev,cur) to (cur,next) if (prev,cur,next) is valid */
        for (int prev = 0; prev < 32; ++prev) {
            for (int cur = 0; cur < 32; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == 0) continue;
                uint8_t  cnt  = prv_trans_count[prev][cur];
                uint8_t *list = prv_transitions[prev][cur];
                for (uint8_t k = 0; k < cnt; ++k) {
                    int next = list[k];
                    ndp[cur][next] += ways;
                }
            }
        }

        /* copy ndp into dp for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final step: validate the last row with a zero row below. */
    seq_t ans = (seq_t)0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            if (!prv_valid[prev][cur][0]) continue;
            ans += dp[prev][cur];
        }
    }

    return ans;
}
